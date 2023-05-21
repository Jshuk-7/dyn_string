#ifndef DYN_STRING_H
#define DYN_STRING_H

#include <string.h>

#ifndef D_MALLOC
#include <stdlib.h>
#define D_MALLOC malloc
#endif

#ifndef D_REALLOC
#include <stdlib.h>
#define D_REALLOC realloc
#endif

#ifndef D_FREE
#include <stdlib.h>
#define D_FREE free
#endif

#ifndef D_ASSERT
#include <assert.h>
#define D_ASSERT assert
#endif

#ifdef DYN_STRING_USE_WIDE
typedef wchar_t char_t;
#else
typedef char char_t;
#endif

typedef struct DString
{
    char_t* data;
    size_t size;
    size_t capacity;
} DString;

DString dstring_create(char_t* data);
DString dstring_create_with_capacity(size_t capacity);
void dstring_destroy(DString* string);
void dstring_realloc(DString* string, size_t new_capacity);
const char_t* dstring_to_cstr(DString* string);
void dstring_push(DString* string, char_t c);
char_t dstring_pop(DString* string);

#endif

#ifdef DYN_STRING_IMPLEMENTATION

DString dstring_create(char_t *data)
{
    DString string;
    string.size = strlen((const char_t *)data);
    string.capacity = string.size * 2;
    string.data = (char_t *)D_MALLOC(sizeof(char_t) * string.capacity);
    D_ASSERT(string.data != NULL);
    memcpy(string.data, data, string.size);
    string.data[string.size] = '\0';
    return string;
}

DString dstring_create_with_capacity(size_t capacity)
{
    DString string;
    string.capacity = capacity;
    string.size = 0;
    string.data = (char_t*)D_MALLOC(sizeof(char_t) * string.capacity);
    D_ASSERT(string.data != NULL);
    return string;
}

void dstring_destroy(DString* string)
{
    string->size = 0;
    string->capacity = 0;
    D_FREE(string->data);
}

void dstring_realloc(DString* string, size_t new_capacity)
{
    D_ASSERT(string->data != NULL);
    string->data = (char_t*)D_REALLOC(string->data, new_capacity);
    D_ASSERT(string->data != NULL);
}

const char_t* dstring_to_cstr(DString* string)
{
    D_ASSERT(string->data[string->size] == '\0');
    return (const char_t*)string->data;
}

void dstring_push(DString* string, char_t c)
{
    if (string->size + 1 >= string->capacity) {
        dstring_realloc(string, string->capacity * 2);
    }

    string->data[string->size] = c;
    string->size++;
    string->data[string->size] = '\0';
}

char_t dstring_pop(DString* string)
{
    D_ASSERT(string->size > 0);
    char_t c = string->data[string->size - 1];
    string->size--;
    string->data[string->size] = '\0';
    return c;
}

#endif