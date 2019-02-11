BITS 64

    xor rax, rax
    sub rax, 1
    xor rcx, rcx

l:
    add rcx, 1
    mov rbx, rax
    sub rbx, rcx
    jnz l

