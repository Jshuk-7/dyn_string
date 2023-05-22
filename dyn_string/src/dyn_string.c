#define DYN_STRING_IMPLEMENTATION
#include "dyn_string.h"

#include <stdio.h>

int main(void)
{
    {
        DString string = dstring_create("Hello, World");
        printf("%s | %zu | %zu\n", dstring_to_cstr(&string), string.size, string.capacity);
        dstring_push(&string, '!');
        printf("%s | %zu | %zu\n", dstring_to_cstr(&string), string.size, string.capacity);
        char_t c = dstring_pop(&string);
        printf("%s | %zu | %zu\n", dstring_to_cstr(&string), string.size, string.capacity);
        dstring_destroy(&string);
    }

    printf("-------------------------------\n");

    {
        DString string = dstring_create("Hello, World!");

        DString sub = dstring_substring(&string, 0, 5);
        dstring_push(&sub, '!');
        printf("%s\n", dstring_to_cstr(&sub));
    }
    return 0;
}