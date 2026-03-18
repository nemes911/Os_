// kernel/include/errno.h

#ifndef _ERRNO_H
#define _ERRNO_H

/* Основные коды ошибок (POSIX-совместимые) */
#define EPERM       1       /* Operation not permitted */
#define ENOENT      2       /* No such file or directory */
#define ESRCH       3       /* No such process */
#define EINTR       4       /* Interrupted system call */
#define EIO         5       /* Input/output error */
#define ENXIO       6       /* No such device or address */
#define E2BIG       7       /* Argument list too long */
#define ENOEXEC     8       /* Exec format error */
#define EBADF       9       /* Bad file descriptor */
#define ECHILD     10       /* No child processes */
#define EAGAIN     11       /* Resource temporarily unavailable */
#define ENOMEM     12       /* Cannot allocate memory */
#define EACCES     13       /* Permission denied */
#define EFAULT     14       /* Bad address */
#define EBUSY      16       /* Device or resource busy */
#define EEXIST     17       /* File exists */
#define ENODEV     19       /* No such device */
#define ENOTDIR    20       /* Not a directory */
#define EISDIR     21       /* Is a directory */
#define EINVAL     22       /* Invalid argument */
#define ENFILE     23       /* Too many open files in system */
#define EMFILE     24       /* Too many open files */
#define ENOTTY     25       /* Inappropriate ioctl for device */
#define EFBIG      27       /* File too large */
#define ENOSPC     28       /* No space left on device */
#define ESPIPE     29       /* Illegal seek */
#define EROFS      30       /* Read-only file system */
#define ENAMETOOLONG 36     /* Filename too long */
#define ENOLCK     37       /* No locks available */

/* Для совместимости с некоторыми функциями */
#define EWOULDBLOCK EAGAIN

/* Глобальная переменная errno (если хочешь использовать как в POSIX) */
/* В ядре часто делают per-task errno, но пока можно оставить глобальную */
extern int errno;

#endif /* _ERRNO_H */