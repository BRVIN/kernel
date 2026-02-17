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
.global stack_bottom, stack_top
stack_bottom:
    .skip 16384
stack_top:

.section .text
.global _start
.type _start, @function
_start:
    cli
    mov $stack_top, %esp # stack initialisation

    lgdt gdt_descriptor         # charger GDTR

    # 0x10 is Kernel Data Segment
    mov $0x10, %ax
    mov %ax, %ds
    mov %ax, %es
    mov %ax, %fs
    mov %ax, %gs

    # 0x18 is Kernel Stack Segment
    mov $0x18, %ax
    mov %ax, %ss
    mov $stack_top, %esp        # recharger stack

    ljmp $0x08, $kernel_main    # sauter vers kernel_main
    # 0x08 is Kernel Code Segment, see below

    # call kernel_main # deprecated call

    cli
1:  hlt
    jmp 1b

.size _start, .-_start

.section .gdt
.align 8
.global gdt_start, gdt_end, gdt_descriptor
#.org 0x800 // recheck this shit even if we dont need it btw

gdt_start:
    # 0x00 - 0o - Null segment # pourquoi faire deja ?
    .quad 0x0000000000000000

    # 0x08 - +8o = 16o - Kernel Code Segment (ring 0)
    .quad 0x00CF9A000000FFFF

    # 0x10 - +8o = 24o - Kernel Data Segment (ring 0)
    .quad 0x00CF92000000FFFF

    # 0x18 - +8o = 32o - Kernel Stack Segment (ring 0)
    .quad 0x00CF92000000FFFF

    # 0x20 - +8o = 40o - User Code Segment (ring 3)
    .quad 0x00CFFA000000FFFF

    # 0x28 - +8o = 48o - User Data Segment (ring 3)
    .quad 0x00CFF2000000FFFF

    # 0x30 - +8o = 56o - User Stack Segment (ring 3)
    .quad 0x00CFF2000000FFFF

    # TODO print check User Segments address

gdt_end: # 0x37 = 55 -> 56o 7 segments × 8o

gdt_descriptor:
    .word gdt_end - gdt_start - 1 # taille (limit)
    .long gdt_start               # base (adresse)
