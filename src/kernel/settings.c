#include <kernel/settings.h>
#include <lib/string.h>

static setting_t settings[MAX_SETTINGS];
static int settings_count = 0;
uint8_t int_subscribers_count = 0;
uint8_t string_subscribers_count = 0;

static settings_int_subscriber_t int_subscribers[MAX_SUBSCRIBERS];
static settings_string_subscriber_t string_subscribers[MAX_SUBSCRIBERS];

void settings_init(void)
{
    settings_count = 0;
    int_subscribers_count = 0;
    string_subscribers_count = 0;

    settings_set_int("mouse.sensitivity", 130);
    settings_set_int("timer.frequency", 1000);
}

static setting_t *find_setting(const char *key)
{
    for (int i = 0; i < settings_count; i++)
        if (strcmp(settings[i].key, key) == 0)
            return &settings[i];

    return 0;
}

int settings_subscribe_int(const char *key, settings_int_callback_t cb)
{
    if (int_subscribers_count < MAX_SUBSCRIBERS)
    {
        int_subscribers[int_subscribers_count].key = key;
        int_subscribers[int_subscribers_count].callback = cb;
        int_subscribers_count++;
        return 0;
    }
    return -1;
}

int settings_subscribe_string(const char *key, settings_string_callback_t cb)
{
    if (string_subscribers_count < MAX_SUBSCRIBERS)
    {
        string_subscribers[string_subscribers_count].key = key;
        string_subscribers[string_subscribers_count].callback = cb;
        string_subscribers_count++;
        return 0;
    }
    return -1;
}

int settings_unsubscribe_int(const char *key, settings_int_callback_t cb)
{
    for (int i = 0; i < int_subscribers_count; i++)
    {
        if (int_subscribers[i].callback == cb && strcmp(int_subscribers[i].key, key) == 0)
        {
            int_subscribers[i] = int_subscribers[--int_subscribers_count];
            return 0;
        }
    }
    return -1;
}

int settings_unsubscribe_string(const char *key, settings_string_callback_t cb)
{
    for (int i = 0; i < string_subscribers_count; i++)
    {
        if (string_subscribers[i].callback == cb && strcmp(string_subscribers[i].key, key) == 0)
        {
            string_subscribers[i] = string_subscribers[--string_subscribers_count];
            return 0;
        }
    }
    return -1;
}

int settings_set_int(const char *key, int value)
{
    setting_t *s = find_setting(key);
    if (!s)
    {
        if (settings_count >= MAX_SETTINGS)
            return -1;
        s = &settings[settings_count++];
        s->key = key;
    }
    s->type = SETTING_INT;
    s->value.int_val = value;

    for (int i = 0; i < int_subscribers_count; i++)
        if (strcmp(int_subscribers[i].key, key) == 0)
            int_subscribers[i].callback(value);

    return 0;
}

int settings_get_int(const char *key, int default_value)
{
    setting_t *s = find_setting(key);
    if (!s || s->type != SETTING_INT)
        return default_value;
    return s->value.int_val;
}

int settings_set_string(const char *key, const char *value)
{
    setting_t *s = find_setting(key);
    if (!s)
    {
        if (settings_count >= MAX_SETTINGS)
            return -1;
        s = &settings[settings_count++];
        s->key = key;
    }
    s->type = SETTING_STRING;
    strncpy(s->value.str_val, value, MAX_STRING_LEN - 1);

    for (int i = 0; i < string_subscribers_count; i++)
        if (strcmp(string_subscribers[i].key, key) == 0)
            string_subscribers[i].callback(value);

    return 0;
}

const char *settings_get_string(const char *key, const char *default_value)
{
    setting_t *s = find_setting(key);
    if (!s || s->type != SETTING_STRING)
        return default_value;
    return s->value.str_val;
}
