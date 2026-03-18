#include "disk.h"
#include "inode.h"
#include "bitmap.h"
#include "supperblock.h"
#include "../pcb/procces.h"
#include "errno.h"        // основной
// или

#define SUPERBLOCK_LBA  0
#define INODE_START_LBA 2
#define MAX_OPEN_FILES 64
#define O_RDONLY    0
#define O_WRONLY    1
#define O_RDWR      2
#define O_APPEND    0x400   // можно добавить позже
#define BLOCK_SIZE  512     // или какое у тебя значение сектора

typedef struct {
    int used;
    uint32_t inode_num;
    uint32_t pos;           // лучше uint64_t, если файлы >4 ГБ
    int mode;
    int flags;
    Procces *owner;
} OpenFile;                 // ← исправленное имя

static OpenFile open_files[MAX_OPEN_FILES];

static int alloc_fd(void){
    for (int i =0; i < MAX_OPEN_FILES; i++){
        if(!open_files[i].used){
            open_files[i].used = 1;
            return i;
        }
    }
    return -1;
}

int fs_close(int fd)
{
    if (fd < 0 || fd >= 8 || current_pcb->fds[fd] == -1)
        return -EBADF;

    int slot = current_pcb->fds[fd];

    open_files[slot].used = 0;
    current_pcb->fds[fd] = -1;
    current_pcb->open_count--;

    return 0;
}

static int find_inode_by_name (const char *name){
    if(!name || !*name) return -1;

    uint8_t root_block[512];

    if(disk_read_sector(1, root_block) != 0)
        return -2;

    char *p = (char*)root_block;
    const char *end = p + 512;

    while (p < end)
    {
        if(*p == 0)break;
        if(strcmp(p, name) == 0){
            p += strlen(p) + 1;
            if(p + 4 > end) break;
            uint32_t ino = *(uint32_t*)p;
            return ino;
        }
        p += strlen(p) + 1 + 4;
    }
    return -1;
}

int fs_create(const char *name, int mode)
{
    if (!current_pcb || !name || !*name) return -EINVAL;

    // 1. Проверяем, существует ли уже файл с таким именем
    if (find_inode_by_name(name) >= 0) return -EEXIST;

    // 2. Выделяем новый inode
    uint32_t new_inode = inode_alloc();     // предполагаем, что у тебя есть inode_alloc()
    if (new_inode == 0) return -ENOSPC;

    inode_t inode;
    memset(&inode, 0, sizeof(inode));
    inode.size = 0;
    // inode.type = ... (если есть тип файла)
    // inode.mode = mode;  // права доступа, если реализуешь

    if (inode_write(new_inode, &inode) != 0) {
        inode_free(new_inode);
        return -EIO;
    }

    // 3. Добавляем запись в root-блок (имя + inode)
    uint8_t root_block[512];
    if (disk_read_sector(1, root_block) != 0) {
        inode_free(new_inode);
        return -EIO;
    }

    // Находим свободное место в root_block
    char *p = (char*)root_block;
    const char *end = p + 512;

    while (p < end && *p != 0) {
        p += strlen(p) + 1 + 4;     // имя + \0 + uint32_t
    }

    if (p + strlen(name) + 1 + 4 > end) {
        inode_free(new_inode);
        return -ENOSPC;     // root переполнен
    }

    strcpy(p, name);
    p += strlen(name) + 1;
    *(uint32_t*)p = new_inode;

    if (disk_write_sector(1, root_block) != 0) {
        inode_free(new_inode);
        return -EIO;
    }

    printf("Created file '%s' with inode %u\n", name, new_inode);
    return 0;
}
int fs_init(void)
{
    int ret;

    disk_init();

    ret = disk_read_sector(SUPERBLOCK_LBA, &superblock);
    if (ret != 0) {
        printf("fs_init: cannot read superblock\n");
        return -EIO;           // ← используем errno-код
    }

    if (superblock.magic != S5FS_MAGIC) {
        printf("fs_init: invalid magic 0x%x (expected 0x%x)\n",
               superblock.magic, S5FS_MAGIC);
        return -EINVAL;
    }

    printf("fs_init: mounted s5fs, blocks: %u, free: %u, inode count: %u\n",
           superblock.total_blocks, superblock.free_blocks, superblock.inode_table_start);

    bitmap_init(&superblock);
    inode_init(&superblock);

    return 0;   // успех
}

int fs_read(int fd, void *buf, size_t count)
{
    if (fd < 0 || fd >= 8 || current_pcb->fds[fd] == -1) return -EBADF;

    int slot = current_pcb->fds[fd];
    OpenFile *of = &open_files[slot];

    if (of->flags & O_WRONLY) return -EBADF;   // открыт только на запись

    inode_t inode;
    if (inode_read(of->inode_num, &inode) != 0)
        return -EIO;

    size_t total_read = 0;
    uint64_t pos = of->pos;

    while (count > 0)
    {
        // находим блок, в котором находится pos
        uint32_t block_idx = pos / BLOCK_SIZE;
        uint32_t offset_in_block = pos % BLOCK_SIZE;

        uint32_t disk_block;

        if (block_idx < 12)
        {
            // direct
            disk_block = inode.direct[block_idx];
        }
        else
        {
            // для простоты пока только direct блоки
            // если нужно indirect — добавляем чтение indirect-блока
            return total_read;  // конец файла или не поддерживается
        }

        if (disk_block == 0) break;   // дырка или конец

        uint8_t block_buf[BLOCK_SIZE];
        if (disk_read_sector(disk_block, block_buf) != 0)
            return -EIO;

        size_t can_read = BLOCK_SIZE - offset_in_block;
        if (can_read > count) can_read = count;
        if (can_read > inode.size - pos) can_read = inode.size - pos;

        memcpy((char*)buf + total_read, block_buf + offset_in_block, can_read);

        total_read += can_read;
        pos += can_read;
        count -= can_read;
    }

    of->pos = pos;
    return total_read;
}

int fs_write(int fd, const void *buf, size_t count)
{
    if (fd < 0 || fd >= 8 || current_pcb->fds[fd] == -1) return -EBADF;

    int slot = current_pcb->fds[fd];
    OpenFile *of = &open_files[slot];

    if (of->flags & O_RDONLY) return -EBADF;

    inode_t inode;
    if (inode_read(of->inode_num, &inode) != 0)
        return -EIO;

    size_t total_written = 0;
    uint64_t pos = of->pos;

    while (count > 0)
    {
        uint32_t block_idx = pos / BLOCK_SIZE;
        uint32_t offset_in_block = pos % BLOCK_SIZE;

        uint32_t disk_block = 0;

        if (block_idx < 12)
        {
            disk_block = inode.direct[block_idx];
            if (disk_block == 0)
            {
                // выделяем новый блок
                disk_block = bitmap_alloc_block();
                if (disk_block == 0) return -ENOSPC;
                inode.direct[block_idx] = disk_block;
            }
        }
        else
        {
            // пока поддерживаем только direct
            return total_written;
        }

        uint8_t block_buf[BLOCK_SIZE];

        // если не новый блок — читаем существующий
        if (offset_in_block != 0 || count < BLOCK_SIZE)
        {
            if (disk_read_sector(disk_block, block_buf) != 0)
                return -EIO;
        }
        else
        {
            memset(block_buf, 0, BLOCK_SIZE);
        }

        size_t can_write = BLOCK_SIZE - offset_in_block;
        if (can_write > count) can_write = count;

        memcpy(block_buf + offset_in_block, (char*)buf + total_written, can_write);

        if (disk_write_sector(disk_block, block_buf) != 0)
            return -EIO;

        total_written += can_write;
        pos += can_write;
        count -= can_write;
    }

    if (pos > inode.size)
        inode.size = pos;

    inode_write(of->inode_num, &inode);

    of->pos = pos;
    return total_written;
}

void fs_init(){

    int ret;

    disk_init();

    ret = disk_read_sector(SUPERBLOCK_LBA, &superblock);
        if(ret != 0){
            printf("fs_init: cannot read superblock\n");
            return -1;
        }

    if(superblock.magic != S5FS_MAGIC){
        printf("fs_init: invalid magic 0x%x (expected 0x%x)\n", superblock.magic, S5FS_MAGIC);
        return -2;
    }

    printf("fs_init: mounted s5fs, blocks: %u, free: %u, inode count: %u\n",
           superblock.total_blocks, superblock.free_blocks, superblock.inode_table_start);

    bitmap_init(&superblock);
    inode_init(&superblock);


    return 0;
}

int fs_lseek(int fd, int64_t offset, int whence)
{
    if (fd < 0 || fd >= 8 || current_pcb->fds[fd] == -1) return -EBADF;

    int slot = current_pcb->fds[fd];
    OpenFile *of = &open_files[slot];

    inode_t inode;
    if (inode_read(of->inode_num, &inode) != 0) return -EIO;

    uint64_t new_pos;

    switch (whence) {
        case 0: new_pos = offset; break;               // SEEK_SET
        case 1: new_pos = of->pos + offset; break;     // SEEK_CUR
        case 2: new_pos = inode.size + offset; break;  // SEEK_END
        default: return -EINVAL;
    }

    if (new_pos > inode.size) new_pos = inode.size;    // не дальше конца

    of->pos = new_pos;
    return (int)new_pos;   // или long long, если offset большой
}

