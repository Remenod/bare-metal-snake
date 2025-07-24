SRC_DIR := uefi-app
BUILD_DIR := build

SRC_C := $(SRC_DIR)/main.c
SRC_ASM := $(SRC_DIR)/start.asm

OBJ_C := $(BUILD_DIR)/main.o
OBJ_ASM := $(BUILD_DIR)/start.o

EFI := $(BUILD_DIR)/boot.efi
LINKER_SCRIPT := $(SRC_DIR)/elf_x86_64_efi.lds

CFLAGS := -I/usr/include/efi -I/usr/include/efi/x86_64 -Wall -Wextra -ffreestanding -fno-stack-protector -mno-red-zone -fshort-wchar
ASMFLAGS := -f elf64

LDFLAGS := -nostdlib -T $(LINKER_SCRIPT) -shared -Bsymbolic -L/usr/lib -L/usr/lib/gnu-efi

.PHONY: all clean

all: $(EFI)

$(BUILD_DIR):
	mkdir -p $@

$(OBJ_C): $(SRC_C) | $(BUILD_DIR)
	clang $(CFLAGS) -c $< -o $@

$(OBJ_ASM): $(SRC_ASM) | $(BUILD_DIR)
	nasm $(ASMFLAGS) $< -o $@

$(EFI): $(OBJ_ASM) $(OBJ_C)
	ld $(LDFLAGS) $^ -o $@

clean:
	rm -rf $(BUILD_DIR)
