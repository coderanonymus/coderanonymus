CROSS ?= x86_64-elf-
CC := $(CROSS)gcc
LD := $(CROSS)ld
OBJCOPY := $(CROSS)objcopy
AS := nasm

CFLAGS := -std=gnu11 -ffreestanding -fno-stack-protector -fno-pic -mno-red-zone -Wall -Wextra -Iinclude -O2
LDFLAGS := -nostdlib -z max-page-size=0x1000 -T linker.ld

KERNEL_OBJS := \
	build/entry.o \
	build/main.o \
	build/vga.o \
	build/keyboard.o \
	build/shell.o \
	build/mm.o \
	build/sched.o \
	build/ipc.o \
	build/process.o \
	build/libc.o \
	build/elf.o \
	build/vfs.o \
	build/net.o \
	build/gui.o \
	build/drivers.o \
	build/platform.o \
	build/debug.o \
	build/log.o

.PHONY: all clean run minimal iso run-iso check-host

all: build/aetheros.img
minimal: all

build:
	mkdir -p build

build/boot.bin: boot/bios/boot.asm | build
	$(AS) -f bin $< -o $@

build/stage2.bin: boot/bios/stage2.asm | build
	$(AS) -f bin $< -o $@

build/entry.o: kernel/entry.asm | build
	$(AS) -f elf64 $< -o $@

build/main.o: kernel/main.c | build
	$(CC) $(CFLAGS) -c $< -o $@

build/vga.o: kernel/vga.c | build
	$(CC) $(CFLAGS) -c $< -o $@

build/keyboard.o: kernel/keyboard.c | build
	$(CC) $(CFLAGS) -c $< -o $@

build/shell.o: kernel/shell.c | build
	$(CC) $(CFLAGS) -c $< -o $@

build/mm.o: kernel/mm/mm.c | build
	$(CC) $(CFLAGS) -c $< -o $@

build/sched.o: kernel/sched/sched.c | build
	$(CC) $(CFLAGS) -c $< -o $@

build/ipc.o: kernel/sched/ipc.c | build
	$(CC) $(CFLAGS) -c $< -o $@

build/process.o: kernel/user/process.c | build
	$(CC) $(CFLAGS) -c $< -o $@

build/libc.o: kernel/user/libc.c | build
	$(CC) $(CFLAGS) -c $< -o $@

build/elf.o: kernel/user/elf.c | build
	$(CC) $(CFLAGS) -c $< -o $@

build/vfs.o: kernel/fs/vfs.c | build
	$(CC) $(CFLAGS) -c $< -o $@

build/net.o: kernel/net/net.c | build
	$(CC) $(CFLAGS) -c $< -o $@

build/gui.o: kernel/gui/gui.c | build
	$(CC) $(CFLAGS) -c $< -o $@

build/drivers.o: kernel/drivers/drivers.c | build
	$(CC) $(CFLAGS) -c $< -o $@

build/platform.o: kernel/arch/x86_64/platform.c | build
	$(CC) $(CFLAGS) -c $< -o $@

build/debug.o: kernel/debug/debug.c | build
	$(CC) $(CFLAGS) -c $< -o $@

build/log.o: kernel/debug/log.c | build
	$(CC) $(CFLAGS) -c $< -o $@

build/kernel.elf: $(KERNEL_OBJS) linker.ld
	$(LD) $(LDFLAGS) -o $@ $(KERNEL_OBJS)

build/kernel.bin: build/kernel.elf
	$(OBJCOPY) -O binary $< $@

build/aetheros.img: build/boot.bin build/stage2.bin build/kernel.bin
	cp build/boot.bin $@
	cat build/stage2.bin >> $@
	cat build/kernel.bin >> $@
	truncate -s 16M $@

iso: build/aetheros.iso

build/aetheros.iso: build/aetheros.img | build
	mkdir -p build/iso
	cp build/aetheros.img build/iso/aetheros.img
	xorriso -as mkisofs \
	  -R -J \
	  -b aetheros.img \
	  -no-emul-boot \
	  -boot-load-size 4 \
	  -boot-info-table \
	  -o $@ build/iso

run: build/aetheros.img
	qemu-system-x86_64 -drive format=raw,file=build/aetheros.img

run-iso: build/aetheros.iso
	qemu-system-x86_64 -cdrom build/aetheros.iso

check-host:
	gcc -std=gnu11 -Wall -Wextra -Iinclude -fsyntax-only \
		kernel/main.c kernel/vga.c kernel/keyboard.c kernel/shell.c \
		kernel/mm/mm.c kernel/sched/sched.c kernel/sched/ipc.c \
		kernel/user/process.c kernel/user/libc.c kernel/user/elf.c \
		kernel/fs/vfs.c kernel/net/net.c kernel/gui/gui.c \
		kernel/drivers/drivers.c kernel/arch/x86_64/platform.c \
		kernel/debug/debug.c kernel/debug/log.c

clean:
	rm -rf build
