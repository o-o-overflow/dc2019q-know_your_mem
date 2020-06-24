
.globl jump_to_shellcode
jump_to_shellcode:
    mov %rsp, %rax
    mov $0, %ax
    mov %rax, %rdi
    mov $0xf000, %rsi
    mov $11, %rax
    syscall


    vzeroall
    xor %rax, %rax
    xor %rbx, %rbx
    xor %rcx, %rcx
    xor %rdx, %rdx
    xor %rsi, %rsi
    xor %r8, %r8
    xor %r9, %r9
    xor %r10, %r10
    xor %r11, %r11
    xor %r12, %r12
    xor %r13, %r13
    xor %r14, %r14
    xor %r15, %r15

    xor %rbp, %rbp
    xor %rsp, %rsp

    jmp *%rdi
