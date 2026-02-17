CC = sudo /home/ubuntu/opt/cross/bin/i686-elf-gcc
AS = sudo /home/ubuntu/opt/cross/bin/i686-elf-as
LD = sudo /home/ubuntu/opt/cross/bin/i686-elf-gcc

CFLAGS = -fno-builtin -fno-exceptions -fno-stack-protector -nostdlib -nodefaultlibs -ffreestanding -O2 -march=i386 -m32 -g -O0
ASFLAGS =
LDFLAGS = -T linker.ld -ffreestanding -O2 -nostdlib -lgcc -march=i386 -fno-builtin -fno-exceptions -fno-stack-protector -nostdlib -nodefaultlibs

INC_DIR = -Iinclude

SRC_C_DIR = src
SRC_C_LIST = 	io.c \
				kernel.c \
				keyboard.c \
				printk.c \
				screen.c \
				draw.c \
				utils.c \
				cursor.c \
				date.c \
				cmds.c

SRC_C = $(addprefix $(SRC_C_DIR)/, $(SRC_C_LIST))

SRC_S = boot.s read_gdtr.s
OBJ = $(SRC_C:.c=.o) $(SRC_S:.s=.o) 
EXEC = ./isodir/boot/myos.elf 

all: $(EXEC)
	grub-mkrescue --compress=xz -o myos.iso isodir 
	#rm -f $(EXEC)

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
	rm -f $(EXEC)

re: fclean
	all

start:
	qemu-system-i386 -cdrom myos.iso

startd:
	qemu-system-i386 -cdrom myos.iso -s -S


iso:
	qemu-system-i386 -cdrom myos.iso

elf:
	qemu-system-i386 -kernel isodir/boot/myos.elf

.PHONY: all clean fclean start
