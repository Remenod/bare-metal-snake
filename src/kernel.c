volatile unsigned short *vga = (volatile unsigned short *)0xB8000;

void put_char(int pos, char c)
{
    vga[pos] = (0x0F << 8) | c;
}

void clear_screen()
{
    unsigned short blank = (0x07 << 8) | ' ';
    for (int i = 0; i < 80 * 25; i++)
    {
        vga[i] = blank;
    }
}

void kernel_main()
{
    put_char(0, 'H');
    put_char(1, 'i');
    while (1)
        ;
}
