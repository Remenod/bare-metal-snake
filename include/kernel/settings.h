#pragma once

#include <lib/types.h>

typedef enum
{
    SETTING_INT,
    SETTING_BOOL,
    SETTING_STRING
} setting_type_t;

#define MAX_SETTINGS 64
#define MAX_STRING_LEN 64
#define MAX_SUBSCRIBERS 8

#define settings_subscribe(key, cb)                            \
    _Generic(cb,                                               \
        settings_string_callback_t: settings_subscribe_string, \
        settings_int_callback_t: settings_subscribe_int)(key, cb)

#define settings_unsubscribe(key, cb)                            \
    _Generic(cb,                                                 \
        settings_string_callback_t: settings_unsubscribe_string, \
        settings_int_callback_t: settings_unsubscribe_int)(key, cb)

typedef struct
{
    const char *key;
    setting_type_t type;
    union
    {
        int int_val;
        int bool_val;
        char str_val[MAX_STRING_LEN];
    } value;
} setting_t;

typedef void (*settings_int_callback_t)(int new_value);
typedef void (*settings_string_callback_t)(const char *new_value);

typedef struct
{
    const char *key;
    settings_int_callback_t callback;
} settings_int_subscriber_t;

typedef struct
{
    const char *key;
    settings_string_callback_t callback;
} settings_string_subscriber_t;

void settings_init();

int settings_subscribe_int(const char *key, settings_int_callback_t cb);
int settings_subscribe_string(const char *key, settings_string_callback_t cb);

int settings_unsubscribe_int(const char *key, settings_int_callback_t cb);
int settings_unsubscribe_string(const char *key, settings_string_callback_t cb);

int settings_set_int(const char *key, int value);
int settings_get_int(const char *key, int default_value);

int settings_set_string(const char *key, const char *value);
const char *settings_get_string(const char *key, const char *default_value);
