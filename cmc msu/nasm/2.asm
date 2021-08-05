%include "io.inc"
CEXTERN fprintf
CEXTERN scanf
CEXTERN malloc
CEXTERN free

section .text
global CMAIN
CMAIN:
    mov ebp, esp
    and esp, 0xfffffff0
    sub esp, 8 ;array pointer + array size
    ;scanf
    push esp
    push fi
    call scanf
    add esp, 8
    ;/scanf
    mov eax, [esp]
    mul dword[argsz]
    ;malloc
    sub esp, 4
    push eax
    call malloc
    add esp, 8
    ;/malloc
    mov [esp+4], eax
    mov esi, [esp]
    mov edi, eax
.loop1:
    ;scanf
    push edi
    push fi
    call scanf
    add esp, 8
    ;/scanf
    add edi, [argsz]
    dec esi
    jnz .loop1
    ;apply
    mov edi, [esp+4]
    mov esi, [esp]
    mov eax, fo
    push eax
    call get_stdout
    push eax
    mov eax, 2
    push eax
    mov eax, fprintf
    push eax
    push esi
    push edi
    call apply
    add esp, 24
    ;/apply
    ;free
    mov eax, [esp+4]
    sub esp, 4
    push eax
    call free
    add esp, 8
    ;/free
    add esp, 8
    mov esp, ebp
    xor eax, eax
    ret

;void apply(int* array, size_t length, void (*fn) (...), int n, ...);
apply:
    sub esp, 16
    mov [esp], ebx
    mov [esp+4], ebp
    mov eax, [esp+24] ;array length
    mul dword[argsz]
    mov ecx, eax
    mov ebx, [esp+28] ;function pointer
.loop1:
    mov [esp+8], ecx
    mov eax, [esp+24] ;array length
    mul dword[argsz]
    sub esp, 4
    mov edx, [esp+24] ;array pointer
    add edx, eax
    sub edx, ecx
    mov edx, [edx]
    mov [esp], edx
    mov eax, [esp+36] ;args count
    mul dword[argsz]
    mov ebp, eax
    sub esp, ebp
.loop2:
    add esp, ebp
    lea edx, [esp+36+eax]
    sub esp, ebp
    mov edx, [edx]
    mov [esp+eax-4], edx
    sub eax, 4
    jnz .loop2
    call ebx
    add ebp, 4
    add esp, ebp
    mov ecx, [esp+8]
    sub ecx, 4
    jnz .loop1
    mov ebp, [esp+4]
    mov ebx, [esp]
    add esp, 16
    ret
    
section .bss
n resd 1
section .rodata
fi db "%d", 0
fo db "%d", 10, 0
argsz dd 4