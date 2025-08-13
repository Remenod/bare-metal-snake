#include <arrlib.h>

bool_t contains_uint8(uint8_t arr[], uint32_t size, uint8_t value)
{
    for (uint32_t i = 0; i < size; i++)
        if (arr[i] == value)
            return true;
    return false;
}

bool_t contains_uint16(uint16_t arr[], uint32_t size, uint16_t value)
{
    for (uint32_t i = 0; i < size; i++)
        if (arr[i] == value)
            return true;
    return false;
}

bool_t contains_uint32(uint32_t arr[], uint32_t size, uint32_t value)
{
    for (uint32_t i = 0; i < size; i++)
        if (arr[i] == value)
            return true;
    return false;
}

bool_t contains_int8(int8_t arr[], uint32_t size, int8_t value)
{
    for (uint32_t i = 0; i < size; i++)
        if (arr[i] == value)
            return true;
    return false;
}

bool_t contains_int16(int16_t arr[], uint32_t size, int16_t value)
{
    for (uint32_t i = 0; i < size; i++)
        if (arr[i] == value)
            return true;
    return false;
}

bool_t contains_int32(int32_t arr[], uint32_t size, int32_t value)
{
    for (uint32_t i = 0; i < size; i++)
        if (arr[i] == value)
            return true;
    return false;
}
