CC = x86_64-elf-gcc
CXX = x86_64-elf-c++
LD = x86_64-elf-gcc
CC_32 = i686-elf-gcc
ASM = nasm
LD_32 = i686-elf-gcc

C_SRC_32 = src/bootstrapper/gdt.c src/bootstrapper/stdio.c src/bootstrapper/bootmain.c
C_SRC = src/kernel/kernel.c
ASM_SRC_BOOTSTRAPPER = src/bootstrapper/boot.S src/bootstrapper/header.S src/bootstrapper/utils.S src/bootstrapper/gdt_fl.S
TARGET_KERN = build/kernel.elf 
TARGET_BOOTSTRAPER = build/bootstrapper.elf

CC_FLAGS = -g -ffreestanding -mcmodel=large
CC_FLAGS_32 = -g -ffreestanding -nostdlib

LD_FLAGS = -T linker_scripts.ld -ffreestanding -O2 -nostdlib -lgcc -g

OBJS_C = $(patsubst src/%.c, build/%.o, $(C_SRC))
OBJS_ASM_BOOTSTRAPPER = $(patsubst src/%.S, build/%.o, $(ASM_SRC_BOOTSTRAPPER))
OBJS_C_32 = $(patsubst src/%.c, build/%.o, $(C_SRC_32))
OBJS_ASM = $(patsubst src/%.S, build/%.o, $(ASM_SRC))

.PHONY: all clean

create_build_dir:
	mkdir -p build/kernel build/bootstrapper

all: $(OBJS_C) $(OBJS_ASM) $(OBJS_ASM_BOOTSTRAPPER) $(OBJS_C_32) create_build_dir
	@echo "Linking bootstrapper"
	$(LD_32) $(LD_FLAGS) $(OBJS_C_32) $(OBJS_ASM_BOOTSTRAPPER) -o $(TARGET_BOOTSTRAPER)
	@echo "Linking Kernel"
	$(LD) $(LD_FLAGS) -o $(TARGET_KERN) $(OBJS_ASM) $(OBJS_C)
	@echo "bootstrapper build complete, placed at $(TARGET_BOOTSTRAPER)"
	@echo "Kernel build complete, placed at $(TARGET)"

# Compile C files to object files
build/kernel/%.o: src/kernel/%.c create_build_dir
	@echo $(C_SRC) 
	@echo "Building 64-bit C file $<"
	$(CC) $(CC_FLAGS) -c $< -o $@

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

iso: $(TARGET_KERN) $(TARGET_BOOTSTRAPER)
	@echo "Creating ISO"
	mkdir -p isodir/boot/grub
	cp $(TARGET_KERN) $(TARGET_BOOTSTRAPER) isodir/boot/
	cp grub.cfg isodir/boot/grub/grub.cfg
	grub-mkrescue -o build/os.iso ./isodir
	@echo "ISO created at build/os.iso"

run: all
	qemu-system-i386 -m 512 -cdrom build/os.iso
