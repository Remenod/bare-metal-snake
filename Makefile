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

LIB_C_SRCS := $(wildcard $(SRC_DIR)/lib/*.c)
LIB_ASM_SRCS := $(wildcard $(SRC_DIR)/lib/*.asm)

LIB_C_OBJS := $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(LIB_C_SRCS))
LIB_ASM_OBJS := $(patsubst $(SRC_DIR)/%.asm,$(BUILD_DIR)/%.o,$(LIB_ASM_SRCS))

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

# Boot sector
$(BOOT_BIN): $(BOOT_SRC) | $(BUILD_DIR)
	$(ASM) -f bin $< -o $@

# Kernel entry (asm)
$(ENTRY_OBJ): $(ENTRY_SRC) | $(BUILD_DIR)
	$(ASM) -f elf32 $< -o $@

# Kernel main (C)
$(KERNEL_OBJ): $(KERNEL_SRC) | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Compile C library files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# Compile ASM library files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.asm | $(BUILD_DIR)
	@mkdir -p $(dir $@)
	$(ASM) -f elf32 $< -o $@

# Link everything into kernel.elf
$(KERNEL_ELF): $(ENTRY_OBJ) $(KERNEL_OBJ) $(LIB_C_OBJS) $(LIB_ASM_OBJS) $(LINKER)
	$(LD) $(LDFLAGS) -o $@ $(ENTRY_OBJ) $(KERNEL_OBJ) $(LIB_C_OBJS) $(LIB_ASM_OBJS)

# Extract flat binary
$(KERNEL_BIN): $(KERNEL_ELF)
	$(OBJCOPY) -O binary $< $@
	$(MAKE) pad_kernel

# Final image: bootloader + kernel
$(IMAGE): $(BOOT_BIN) $(KERNEL_BIN)
	cat $^ > $@

# Pad kernel to multiple of 512
pad_kernel:
	@size=$$(stat -c%s $(KERNEL_BIN)); \
	pad=$$(( (512 - (size % 512)) % 512 )); \
	if [ $$pad -ne 0 ]; then \
		dd if=/dev/zero bs=1 count=$$pad >> $(KERNEL_BIN); \
	fi

run: $(IMAGE)
	qemu-system-i386 -drive file=$(IMAGE),format=raw

clean:
	rm -rf $(BUILD_DIR)
