#include <lib/string.h>

int strlen(const char *str)
{
    int len = 0;
    while (str[len] != '\0')
        len++;
    return len;
}

int strcpy(char *dst, const char *src)
{
    int i = 0;
    while ((dst[i] = src[i]) != '\0')
        i++;
    return i;
}

void strcat(char *dst, const char *src)
{
    int dst_len = strlen(dst);
    int i = 0;
    while ((dst[dst_len + i] = src[i]) != '\0')
        i++;
}

char *int_to_str(int value, char *str)
{
    char *p = str;
    char *p1, *p2;
    int is_negative = 0;

    if (value < 0)
    {
        is_negative = 1;
        value = -value;
    }

    do
    {
        *p++ = (value % 10) + '0';
        value /= 10;
    } while (value);

    if (is_negative)
        *p++ = '-';

    *p = '\0';

    p1 = str;
    p2 = p - 1;
    while (p1 < p2)
    {
        char tmp = *p1;
        *p1++ = *p2;
        *p2-- = tmp;
    }

    return str;
}
