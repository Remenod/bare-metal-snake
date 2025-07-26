BOOT_DIR := boot
SRC_DIR := src
BUILD_DIR := build
INCLUDE_DIR := include

BOOT_SRC := $(BOOT_DIR)/boot.asm
ENTRY_SRC := $(SRC_DIR)/kernel/kernel_entry.asm
KERNEL_SRC := $(SRC_DIR)/kernel/kernel.c
LINKER := $(SRC_DIR)/kernel/linker.ld

BOOT_BIN := $(BUILD_DIR)/boot.bin
ENTRY_OBJ := $(BUILD_DIR)/entry.o
KERNEL_OBJ := $(BUILD_DIR)/kernel.o
KERNEL_ELF := $(BUILD_DIR)/kernel.elf
KERNEL_BIN := $(BUILD_DIR)/kernel.bin
IMAGE := $(BUILD_DIR)/snake.img

LIB_SRCS := $(wildcard $(SRC_DIR)/lib/*.c)
LIB_OBJS := $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(LIB_SRCS))

ASM := nasm
CC := i386-elf-gcc
LD := i386-elf-ld
OBJCOPY := i386-elf-objcopy

CFLAGS := -ffreestanding -O2 -Wall -Wextra -m32 -I$(INCLUDE_DIR)
LDFLAGS := -T $(LINKER)

.PHONY: all clean run pad_kernel

all: $(IMAGE)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BOOT_BIN): $(BOOT_SRC) | $(BUILD_DIR)
	$(ASM) -f bin $(BOOT_SRC) -o $(BOOT_BIN)

$(ENTRY_OBJ): $(ENTRY_SRC) | $(BUILD_DIR)
	$(ASM) -f elf32 $(ENTRY_SRC) -o $(ENTRY_OBJ)

$(KERNEL_OBJ): $(KERNEL_SRC) | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $(KERNEL_SRC) -o $(KERNEL_OBJ)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(KERNEL_ELF): $(ENTRY_OBJ) $(KERNEL_OBJ) $(LIB_OBJS) $(LINKER)
	$(LD) $(LDFLAGS) -o $(KERNEL_ELF) $(ENTRY_OBJ) $(KERNEL_OBJ) $(LIB_OBJS)

$(KERNEL_BIN): $(KERNEL_ELF)
	$(OBJCOPY) -O binary $(KERNEL_ELF) $(KERNEL_BIN)
	$(MAKE) pad_kernel

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
