.section .text
.globl gdt_flush
.extern gdt_descriptor

gdt_flush:
    lgdt gdt_descriptor

    mov $0x10, %ax
    mov %ax, %ds
    mov %ax, %es
    mov %ax, %fs
    mov %ax, %gs
    mov %ax, %ss

    ljmp $0x08, $flush
flush:
    ret
