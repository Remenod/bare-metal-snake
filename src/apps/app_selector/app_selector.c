#include <stdint.h>
#include <screen.h>
#include <keyboard.h>
#include <string.h>
#include <stdbool.h>
#include "../snake/snake.h"
#include "../text_sandbox/text_sandbox.h"

typedef struct
{
    const char *name;
    void (*entry_point)();
} App;

static App apps[] = {
    {"Text Sandbox", text_sandbox_main},
    {"Snake", snake_main}};

#define APP_COUNT (sizeof(apps) / sizeof(App))

void app_selector()
{
    clear_screen();
    print("=== Application Selector ===\n\n");

    for (uint32_t i = 0; i < APP_COUNT; i++)
    {
        print_dec(i + 1);
        print(". ");
        print(apps[i].name);
        print("\n");
    }

    print("\nSelect app: ");

    int choice = read_number();
    if (choice > 0 && choice <= APP_COUNT)
    {
        clear_screen();
        apps[choice - 1].entry_point();
    }
    else
    {
        print("\nInvalid selection!\n");
    }
}
