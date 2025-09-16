CC = sudo /home/ubuntu/opt/cross/bin/i686-elf-gcc
AS = sudo /home/ubuntu/opt/cross/bin/i686-elf-as
LD = sudo /home/ubuntu/opt/cross/bin/i686-elf-gcc

CFLAGS = -fno-builtin -fno-exceptions -fno-stack-protector -nostdlib -nodefaultlibs -ffreestanding -O2 -march=i386 -m32 
ASFLAGS =
LDFLAGS = -T linker.ld -ffreestanding -O2 -nostdlib -lgcc -march=i386 -fno-builtin -fno-exceptions -fno-stack-protector -nostdlib -nodefaultlibs

INC_DIR = -Iinclude

SRC_C_DIR = src
SRC_C_LIST = gdt.c \
						 io.c kernel.c \
						 keyboard.c \
						 printk.c \
						 screen.c \
						 draw.c \
						 utils.c \
						 cursor.c \
						 date.c \
						 cmds.c

SRC_C = $(addprefix $(SRC_C_DIR)/, $(SRC_C_LIST))

SRC_S = boot.s gdt_load.s gdt_verify.s  gdt.s
OBJ = $(SRC_C:.c=.o) $(SRC_S:.s=.o) 
EXEC = ./isodir/boot/myos.bin 

all: $(EXEC)
	grub-mkrescue --compress=xz -o myos.iso isodir 
	rm -f $(EXEC)

%.o: %.c 
	$(CC) $(CFLAGS) $(INC_DIR) -c $< -o $@ 

%.o: %.s 
	$(AS) $(ASFLAGS) $< -o $@ 

$(EXEC): $(OBJ) 
	$(LD) $(LDFLAGS) -o $@ $^ 

clean: 
	rm -f $(OBJ)

fclean: clean
	rm -f myos.iso

start:
	qemu-system-i386 -cdrom myos.iso

.PHONY: all clean fclean start
