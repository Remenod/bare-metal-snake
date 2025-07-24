volatile unsigned short *video_memory = (unsigned short *)0xB8000;

void put_char(int pos, char c, unsigned char color)
{
    video_memory[pos] = (color << 8) | c;
}

void efi_main(void *ImageHandle, void *SystemTable)
{
    (void)ImageHandle; // warning ignore
    (void)SystemTable; // warning ignore

    put_char(0, 'A', 0x0F);
}
