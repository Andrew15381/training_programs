section .text
global f1
f1:
    finit
    fld1
    fldl2e
    fdivp
    fld qword[esp+4]
    fyl2x
    ret
global f2
f2:
    finit
    sub esp, 4
    mov dword[esp], 14
    fild dword[esp]
    mov dword[esp], -2
    fild dword[esp]
    add esp, 4
    fld qword[esp+4]
    fmulp
    faddp
    ret
global f3
f3:
    finit
    sub esp, 4
    mov dword[esp], 6
    fild dword[esp]
    fld1
    mov dword[esp], 2
    fild dword[esp]
    add esp, 4
    fld qword[esp+4]
    fsubp
    fdivp
    faddp
    ret
global df1
df1:
    finit
    fld1
    fld qword[esp+4]
    fdivp
    ret
global df2
df2:
    finit
    sub esp, 4
    mov dword[esp], -2
    fild dword[esp]
    add esp, 4
    ret
global df3
df3:
    finit
    sub esp, 4
    fld1
    mov dword[esp], 2
    fild dword[esp]
    add esp, 4
    fld qword[esp+4]
    fsubp
    fld st0
    fmulp
    fdivp
    ret