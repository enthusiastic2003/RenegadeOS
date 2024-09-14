CC_32 = i686-elf-gcc
ASM = nasm
LD_32 = i686-elf-gcc

C_SRC_32 = src/bootstrapper/gdt.c src/bootstrapper/stdio.c src/bootstrapper/bootmain.c
ASM_SRC = src/bootstrapper/boot.S src/bootstrapper/header.S src/bootstrapper/utils.S src/bootstrapper/gdt_fl.S
TARGET = build/kernel.elf

CC_FLAGS_32 = -g -ffreestanding -nostdlib

LD_FLAGS = -T linker_scripts.ld -ffreestanding -O2 -nostdlib -lgcc -g

OBJS_ASM = $(patsubst src/%.S, build/%.o, $(ASM_SRC))
OBJS_C_32 = $(patsubst src/%.c, build/%.o, $(C_SRC_32))

.PHONY: all clean

create_build_dir:
	mkdir -p build/bootstrapper 

all: $(OBJS_ASM)  $(OBJS_C_32) create_build_dir
	@echo "Linking kernel"
	$(LD_32)  $(OBJS_C_32) $(OBJS_ASM) -o $(TARGET) $(LD_FLAGS)
	@echo "Kernel build complete, placed at $(TARGET)"

build/bootstrapper/%.o: src/bootstrapper/%.c create_build_dir
	@echo "Building 32-bit C file $<"
	$(CC_32) $(CC_FLAGS_32) -c $< -o $@

# Assemble ASM files to object files
build/bootstrapper/%.o: src/bootstrapper/%.S $(ASM_SRC_BOOTSTRAPPER) create_build_dir
	@echo "Assembling ASM file $<"
	$(ASM) -f elf32 $< -o $@

clean: 
	@echo "Cleaning build directory"
	rm -rf build/*
	@echo "Cleaned build directory"

iso: $(TARGET)
	@echo "Creating ISO"
	mkdir -p isodir/boot/grub
	cp $(TARGET) isodir/boot/
	cp grub.cfg isodir/boot/grub/grub.cfg
	grub2-mkrescue -o build/os.iso ./isodir
	@echo "ISO created at build/os.iso"

run: all
	qemu-system-i386 -accel kvm -m 512 -cdrom build/os.iso
