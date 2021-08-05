%include "io.inc"
CEXTERN fopen
CEXTERN fclose
CEXTERN fscanf
CEXTERN fprintf
CEXTERN malloc
CEXTERN free

section .text
list_init:
    sub esp, 12
    sub esp, 4
    mov dword[esp], 12
    call malloc
    add esp, 16
    mov ecx, [esp+4]
    mov [eax], ecx
    mov dword[eax+4], 0
    mov dword[eax+8], 0
    ret
    
list_add:
    sub esp, 12
    sub esp, 4
    mov dword[esp], 12
    call malloc
    add esp, 16
    mov ecx, [esp+4]
    mov [eax+4], ecx
    mov ecx, [ecx+8]
    mov [eax+8], ecx
    mov ecx, [esp+4]
    mov [ecx+8], eax
    mov ecx, [esp+8]
    mov [eax], ecx
    ret
    
list_destroy:
    push ebx
    push ebp
    mov ebx, [esp+12]
.loop1:
    test ebx, ebx
    jz .exit1
    sub esp, 12
    mov ebp, [ebx+8]
    push ebx
    call free
    add esp, 16
    mov ebx, ebp
    jmp .loop1
.exit1:
    pop ebp
    pop ebx
    ret
   
apply:
    mov eax, [esp+4]
    mov ecx, [esp+8]
    cmp eax, ecx
    jne .else1
.if1:
    ret
.else1:
    mov ecx, [ecx+4]
    mov edx, [esp+12]
    mov edx, [edx+8]
    mov [ecx+8], edx
    test edx, edx
    jz .else2
.if2:
    mov [edx+4], ecx
.else2:
    mov ecx, [esp+8]
    mov dword[ecx+4], 0
    mov edx, [esp+12]
    mov [edx+8], eax
    mov [eax+4], edx
    mov eax, [esp+8]
    ret
    
print:
    push ebx
    sub esp, 8
    sub esp, 8
    mov dword[esp], fno
    mov dword[esp+4], mo
    call fopen
    add esp, 16
    mov [output], eax
    mov ebx, [esp+8]
.loop1:
    test ebx, ebx
    jz .exit1
    sub esp, 4
    sub esp, 12
    mov eax, [output]
    mov [esp], eax
    mov dword[esp+4], fo
    mov eax, [ebx]
    mov [esp+8], eax
    call fprintf
    add esp, 16
    mov ebx, [ebx+8]
    jmp .loop1
.exit1:
    mov eax, [output]
    sub esp, 12
    push eax
    call fclose
    add esp, 16
    pop ebx
    ret
    
;N     esp+16
;M     esp+12
;head  esp+8
;temp  esp+4
;links esp
global CMAIN
CMAIN:
    mov ebp, esp
    and esp, 0xfffffff0
    sub esp, 8
    sub esp, 8
    mov dword[esp], fni
    mov dword[esp+4], mi
    call fopen
    add esp, 8
    mov [input], eax
    sub esp, 16
    mov eax, [input]
    mov [esp], eax
    mov dword[esp+4], fi
    lea eax, [esp+20]
    mov [esp+8], eax
    lea eax, [esp+16]
    mov [esp+12], eax
    call fscanf
    add esp, 4
    mov eax, [esp+16]
    sal eax, 2
    push eax
    call malloc
    add esp, 4
    mov [esp], eax
    mov eax, 1
    push eax
    call list_init
    add esp, 4
    mov [esp+8], eax
    mov [esp+4], eax
    mov ebx, [esp]
    mov [ebx], eax
    mov ebx, 2
.loop1:
    cmp ebx, [esp+16]
    jg .break1
    mov eax, [esp+4]
    sub esp, 8
    mov [esp], eax
    mov [esp+4], ebx
    call list_add
    add esp, 8
    mov [esp+4], eax
    mov ecx, [esp]
    mov [ecx+4*ebx-4], eax
    inc ebx
    jmp .loop1
.break1:
    sub esp, 8
    mov ebx, [esp+20]
.loop2:
    sub esp, 16
    mov eax, [input]
    mov [esp], eax
    mov dword[esp+4], fi
    lea eax, [esp+20]
    mov [esp+8], eax
    lea eax, [esp+16]
    mov [esp+12], eax
    call fscanf
    add esp, 4
    mov eax, [esp+28]
    mov [esp], eax
    mov eax, [esp+20]
    mov ecx, [esp+16]
    mov eax, [eax+4*ecx-4]
    mov [esp+4], eax
    mov eax, [esp+20]
    mov ecx, [esp+12]
    mov eax, [eax+4*ecx-4]
    mov [esp+8], eax
    call apply
    add esp, 12
    mov [esp+16], eax
    dec ebx
    test ebx, ebx
    jnz .loop2
    add esp, 8
    mov eax, [input]
    push eax
    call fclose
    add esp, 4
    mov eax, [esp+8]
    push eax
    call print
    call list_destroy
    mov eax, [esp+4]
    mov [esp], eax
    call free
    add esp, 4
    mov esp, ebp
    xor eax, eax
    ret
    
section .rodata
mi db "r", 0
mo db "w", 0
fni db "input.txt", 0
fno db "output.txt", 0
fo db "%d ", 0
fi db "%d%d", 0

section .bss
output resd 1
input resd 1