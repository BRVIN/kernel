CC = /home/ubuntu/opt/cross/bin/i686-elf-gcc
AS = /home/ubuntu/opt/cross/bin/i686-elf-as
LD = /home/ubuntu/opt/cross/bin/i686-elf-gcc

CFLAGS = -march=i386 -fno-builtin -fno-stack-protector -nostdlib -nodefaultlibs -ffreestanding -O0 -g
ASFLAGS = -march=i386
LDFLAGS = -T linker.ld -nostdlib -nodefaultlibs

INC_DIR = -Iinclude

SRC_C_DIR = src
SRC_C_LIST = 	io.c \
				kernel.c \
				keyboard.c \
				screen.c \
				draw.c \
				utils.c \
				date.c \
				cmds.c
SRC_C = $(addprefix $(SRC_C_DIR)/, $(SRC_C_LIST))

SRC_S = boot.s

OBJ = $(SRC_C:.c=.o) $(SRC_S:.s=.o) 
ELF = ./isodir/boot/myos.elf 
NAME = myos.iso

all: $(NAME)

%.o: %.c 
	$(CC) $(CFLAGS) $(INC_DIR) -c $< -o $@ 

%.o: %.s 
	$(AS) $(ASFLAGS) $< -o $@ 

$(ELF): $(OBJ) linker.ld
	$(LD) $(LDFLAGS) -o $@ $(OBJ)

$(NAME): $(ELF)
	grub-mkrescue --compress=xz -o $(NAME) isodir 

clean:
	rm -f $(OBJ)
	rm -f $(ELF)

fclean: clean
	rm -f $(NAME)

re: fclean all

start:
	qemu-system-i386 -cdrom $(NAME)

debug: $(NAME)
	@echo "Starting QEMU with GDB stub..."
	@echo "Waiting for GDB connection on localhost:1234"
	qemu-system-i386 -cdrom $(NAME) -s -S &
	@sleep 2
	gdb $(ELF) \
		-ex "target remote localhost:1234" \
		-ex "break kernel_main" \
		-ex "set confirm off" \
		-ex "continue"

.PHONY: all clean fclean re start debug
