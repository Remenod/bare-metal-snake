SRC_DIR := src
BUILD_DIR := build

BOOT := $(SRC_DIR)/boot.asm
ENTRY := $(SRC_DIR)/kernel_entry.asm
KERNEL := $(SRC_DIR)/kernel.c
LINKER := $(SRC_DIR)/linker.ld

BOOT_BIN := $(BUILD_DIR)/boot.bin
ENTRY_OBJ := $(BUILD_DIR)/entry.o
KERNEL_OBJ := $(BUILD_DIR)/kernel.o
KERNEL_ELF := $(BUILD_DIR)/kernel.elf
KERNEL_BIN := $(BUILD_DIR)/kernel.bin

IMAGE := $(BUILD_DIR)/image.img

ASM := nasm
CC := i386-elf-gcc
LD := i386-elf-ld
OBJCOPY := i386-elf-objcopy

CFLAGS := -ffreestanding -m32 -c
LDFLAGS := -T $(LINKER)

all: $(IMAGE)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BOOT_BIN): $(BOOT) | $(BUILD_DIR)
	$(ASM) -f bin $(BOOT) -o $(BOOT_BIN)

$(ENTRY_OBJ): $(ENTRY) | $(BUILD_DIR)
	$(ASM) -f elf32 $(ENTRY) -o $(ENTRY_OBJ)

$(KERNEL_OBJ): $(KERNEL) | $(BUILD_DIR)
	$(CC) $(CFLAGS) $(KERNEL) -o $(KERNEL_OBJ)

$(KERNEL_ELF): $(ENTRY_OBJ) $(KERNEL_OBJ) $(LINKER)
	$(LD) $(LDFLAGS) -o $(KERNEL_ELF) $(ENTRY_OBJ) $(KERNEL_OBJ)

$(KERNEL_BIN): $(KERNEL_ELF)
	$(OBJCOPY) -O binary $(KERNEL_ELF) $(KERNEL_BIN)
	make pad_kernel

$(IMAGE): $(BOOT_BIN) $(KERNEL_BIN)
	cat $(BOOT_BIN) $(KERNEL_BIN) > $(IMAGE)

pad_kernel:
	@size=$$(stat -c%s $(KERNEL_BIN)); \
	pad=$$(( (512 - (size % 512)) % 512 )); \
	if [ $$pad -ne 0 ]; then \
	  dd if=/dev/zero bs=1 count=$$pad >> $(KERNEL_BIN); \
	fi

run: $(IMAGE)
	qemu-system-i386 -drive file=$(IMAGE),format=raw,if=floppy

clean:
	rm -rf $(BUILD_DIR)
