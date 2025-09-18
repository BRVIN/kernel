.section .bss
.comm gdtr_var, 6, 2   # 2 octets d'alignement, taille 6 (2+4)

.section .text
.globl read_gdtr
.extern gdtr_var

read_gdtr:
    sgdt gdtr_var
    ret
