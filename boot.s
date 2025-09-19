.set ALIGN, 1<<0
.set MEMINFO, 1<<1
.set FLAGS, ALIGN | MEMINFO
.set MAGIC, 0x1BADB002
.set CHECKSUM, -(MAGIC + FLAGS)

.section .multiboot
.align 4
.long MAGIC
.long FLAGS
.long CHECKSUM

.section .bss
.align 16
stack_bottom:
    .skip 16384
stack_top:

.section .text
.global _start
.type _start, @function
_start:
    mov $stack_top, %esp

    call kernel_main

    cli
1:  hlt
    jmp 1b

.size _start, .-_start

.section .gdt
.align 8
.global gdt_start, gdt_end, gdt_descriptor
.org 0x800

gdt_start:
    # Null segment
    .long 0x00000000
    .long 0x00000000

    # Code segment 0x00CF9A000000FFFF
    .long 0x0000FFFF   # low 32 bits
    .long 0x00CF9A00   # high 32 bits

    # Data segment 0x00CF92000000FFFF
    .long 0x0000FFFF
    .long 0x00CF9200
gdt_end:

gdt_descriptor:
    .word gdt_end - gdt_start - 1 # taille (limit)
    .long gdt_start               # base (adresse)
