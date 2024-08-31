CC = x86_64-elf-gcc
CXX = x86_64-elf-c++
LD = x86_64-elf-gcc
ASM = nasm

C_SRC = src/kernel/kernel_main.c src/kernel/gdt.c
ASM_SRC = src/boot.S src/header.S
TARGET = build/kernel.elf

CC_FLAGS = -g -ffreestanding -mcmodel=large
LD_FLAGS = -T linker_scripts.ld -ffreestanding -O2 -nostdlib -lgcc 

OBJS_C = $(patsubst src/%.c, build/%.o, $(C_SRC))
OBJS_ASM = $(patsubst src/%.S, build/%.o, $(ASM_SRC))

.PHONY: all clean

create_build_dir:
	mkdir -p build/kernel

all: $(OBJS_C) $(OBJS_ASM) create_build_dir
	@echo "Linking Kernel"
	$(LD) $(LD_FLAGS) -o $(TARGET) $(OBJS_ASM) $(OBJS_C)
	@echo "Kernel build complete, placed at $(TARGET)"

# Compile C files to object files
build/kernel/%.o: src/kernel/%.c create_build_dir
	@echo $(C_SRC) 
	@echo "Building C file $<"
	$(CC) $(CC_FLAGS) -c $< -o $@

# Assemble ASM files to object files
build/%.o: src/%.S $(ASM_SRC) create_build_dir
	@echo "Assembling ASM file $<"
	$(ASM) -f elf64 $< -o $@


clean: 
	@echo "Cleaning build directory"
	rm -rf build/*
	@echo "Cleaned build directory"

iso: $(TARGET)
	@echo "Creating ISO"
	mkdir -p isodir/boot/grub
	cp $(TARGET) isodir/boot/kernel.elf
	cp grub.cfg isodir/boot/grub/grub.cfg
	grub-mkrescue -o build/os.iso ./isodir
	@echo "ISO created at build/os.iso"

run: all
	qemu-system-i386 -cdrom build/os.iso
