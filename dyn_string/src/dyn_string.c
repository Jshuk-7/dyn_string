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
        printf("Substring: %s\n", dstring_to_cstr(&sub));
    }

    printf("-------------------------------\n");

    {
        DString string1 = dstring_create("Hello, World!");
        DString string2 = dstring_create("Hello, World!");
        int equal = dstring_cmp(&string1, &string2);
        printf("Equal: %i\n", equal);
    }
    
    return 0;
}