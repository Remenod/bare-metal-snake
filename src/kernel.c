volatile unsigned short *vga = (volatile unsigned short *)0xB8000;

void put_char(int pos, char c)
{
    vga[pos] = (0x0F << 8) | c;
}

void put_string(int start_pos, char text[])
{
    int endpoint = start_pos + strlen(text);
    for (int i = start_pos; i < endpoint; i++)
    {
        put_char(i, text[i - start_pos]);
    }
}

void clear_screen()
{
    unsigned short blank = (0x07 << 8) | ' ';
    for (int i = 0; i < 80 * 25; i++)
    {
        vga[i] = blank;
    }
}

int strlen(const char *s)
{
    int len = 0;
    while (s[len] != '\0')
    {
        len++;
    }
    return len;
}

void kernel_main()
{
    char hello[] = "Hello, world!";

    put_string(80 * 25 / 2 - strlen(hello) / 2, hello);
    while (1)
        ;
}