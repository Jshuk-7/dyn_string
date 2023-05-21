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

        size_t pos = dstring_find(&string, 'C');
        if (pos != DYN_STRING_NPOS) {
            printf("%zu\n", pos);
        }
    }
    return 0;
}