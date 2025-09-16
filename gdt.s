    .section .data
    .align 8
    .org 0x800
    
gdt_start:
    .long 0x0
    .long 0x0        # null descriptor
gdt_end:

gdt_descriptor:
    .short gdt_end - gdt_start - 1
    .long gdt_start

    .section .text
    .globl gdt_init
gdt_init:
    lgdt gdt_descriptor
    ret
