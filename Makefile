C = sudo /home/ubuntu/opt/cross/bin/i686-elf-gcc
AS = sudo /home/ubuntu/opt/cross/bin/i686-elf-as
LD = sudo /home/ubuntu/opt/cross/bin/i686-elf-gcc

CFLAGS = -m32 -std=gnu99 -g -gsplit-dwarf -ffreestanding -O2 -Wall -Wextra -march=i386 -fno-builtin -fno-exceptions -fno-stack-protector -nostdlib -nodefaultlibs -fcf-protection=none
ASFLAGS =
LDFLAGS = -T linker.ld -g -ffreestanding -O2 -nostdlib -lgcc -march=i386 -fno-builtin -fno-exceptions -fno-stack-protector -fno-rtti -nostdlib -nodefaultlibs

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

SRC_S = boot.s gdt_load.s gdt_verify.s 
OBJ = $(SRC_C:.c=.o) $(SRC_S:.s=.o) 
EXEC = myos.bin 

all: $(EXEC) 

%.o: %.c 
	$(CC) $(CFLAGS) $(INC_DIR) -c $< -o $@ 

%.o: %.s 
	$(AS) $(ASFLAGS) $< -o $@ 

$(EXEC): $(OBJ) 
	$(LD) $(LDFLAGS) -o $@ $^ 

clean: 
	rm -f $(OBJ)
	rm -f src/*.dwo

fclean:
	rm -f $(OBJ) $(EXEC) 
	rm -f src/*.dwo

.PHONY: all clean 
