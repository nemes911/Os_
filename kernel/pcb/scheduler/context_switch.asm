BITS 32

global context_switch

context_switch:


    pusha

    mov eax, [esp + 36]
    mov[eax], esp

    mov eax, [esp + 40]
    mov esp, [eax]

    popa 
    ret