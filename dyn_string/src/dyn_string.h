#ifndef DYN_STRING_H
#define DYN_STRING_H

#include <string.h>
#include <stdint.h>

/*
 * To begin using the dyn_string library:
 *
 * 1. Copy and paste this file into your project
 *
 * 2. In 'one' .c or .cpp file type the following lines
 *    #define DYN_STRING_IMPLEMENTATION
 *    #include "dyn_string.h"
 *
 * 3. Then whenever you need access to the library in another .c/.cpp file
 *    just '#include "dyn_string.h"' and you're off!
 *
 * Things to note:
 * 
 * 1. The string will always be null terminated, so long as you don't
 *    mess with the 'data' pointer. So to stay safe,
 *    never access the data pointer directly, always try to use a library function.
 *    "you mean I have to call a function to do ...?"
 *    yes sir, this is c after all
 * 
 * 2. You can however access the size and capacity of the string if need be.
 *    These members should also never be modified directly, only through a library function.
 */

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

#define DYN_STRING_NPOS 1728772372

typedef struct DString
{
    char_t* data;
    size_t size;
    size_t capacity;
} DString;

/// @brief Creates a new heap allocated string from 'data'. The capacity will default to double the size of 'data'.
/// @param data the data to store in the string
/// @return a new string
DString dstring_create(const char_t* data);

/// @brief Creates a new heap allocated string.
/// @param capacity the capacity of the string
/// @return an empty string with 'capacity'
DString dstring_create_with_capacity(size_t capacity);

/// @brief Frees the strings buffer.
/// @param string said string
void dstring_destroy(DString* string);

/// @brief Reallocates the strings buffer to 'new_capacity'.
/// @param string said string
/// @param new_capacity the new capacity of the string
void dstring_realloc(DString* string, size_t new_capacity);

/// @brief Reserves space for 'n' more characters in the strings buffer. This may cause a reallocation.
/// @param string said string
/// @param n the number of characters to reserve space for
void dstring_reserve(DString* string, size_t n);

/// @brief Checks if the string is empty.
/// @param string said string
/// @return 1 if empty otherwise 0
int dstring_empty(DString* string);

/// @brief Empties the strings buffer. Size gets set to 0.
/// @param string said string
void dstring_clear(DString* string);

/// @brief Reduces capacity of the string to fit its current size. 
/// @param string said string
void dstring_shrink_to_fit(DString *string);

/// @brief Converts the strings buffer to a c string.
/// @param string said string
/// @return the strings buffer as a null terminated c string
const char_t* dstring_to_cstr(DString* string);

/// @brief Gets a substring from a string.
/// @param string the string to extract from
/// @param offset where to start the substring
/// @param count the length of the substring
/// @return a new string with capacity of 'count'
DString dstring_substring(DString* string, size_t offset, size_t count);

/// @brief Pushes a character to the strings buffer. This will cause a reallocation if pushing exceeds the strings capacity.
/// @param string said string
/// @param c the character to push
void dstring_push(DString* string, char_t c);

/// @brief Pops a character from the strings buffer. Decreases size by 1.
/// @param string said string
/// @return the last character in the strings buffer
char_t dstring_pop(DString* string);

/// @brief Gets the first character of the string.
/// @param string said string
/// @return the first character in the strings buffer
char_t dstring_front(DString* string);

/// @brief Gets the last character of the string.
/// @param string said string
/// @return the last character in the strings buffer
char_t dstring_back(DString* string);

/// @brief Shifts all elements of string by 'amount'.
/// @param string said string
/// @param offset where to start shifting
/// @param amount the amount of elements to shift by
void dstring_shift(DString *string, size_t offset, int32_t amount);

/// @brief Replaces a substring within the string. If 'new_val' and 'old_val' are not the same length, the string will resize accordingly. This may cause a reallocation.
/// @param string said string
/// @param offset where to start replacing characters
/// @param old_val the old value
/// @param new_val the new value
void dstring_replace(DString* string, size_t offset, const char_t* old_val, const char_t* new_val);

/// @brief Finds the first instance of 'c' in string.
/// @param string said string
/// @param c the character to search for
/// @return position of 'c' in string if found otherwise returns DYN_STRING_NPOS
size_t dstring_find(DString* string, char_t c);

/// @brief Finds the last instance of 'c' in string.
/// @param string said string
/// @param c the character to search for
/// @return position of 'c' in string if found otherwise returns DYN_STRING_NPOS
size_t dstring_reverse_find(DString *string, char_t c);

/// @brief Compares two strings.
/// @param lhs left operand
/// @param rhs right operand
/// @return 1 if strings are equal otherwise 0
int dstring_cmp(DString* lhs, DString* rhs);

#endif

#ifdef DYN_STRING_IMPLEMENTATION

DString dstring_create(const char_t* data)
{
    DString string;
    string.size = strlen((const char_t *)data);
    string.data = NULL;
    dstring_realloc(&string, string.size * 2);
    memcpy(string.data, data, string.size);
    string.data[string.size] = '\0';
    return string;
}

DString dstring_create_with_capacity(size_t capacity)
{
    DString string;
    string.size = 0;
    string.data = NULL;
    dstring_realloc(&string, capacity);
    return string;
}

void dstring_destroy(DString* string)
{
    string->size = 0;
    string->capacity = 0;
    D_FREE(string->data);
    string->data = NULL;
}

void dstring_realloc(DString* string, size_t new_capacity)
{
    if (string->data == NULL) {
        string->data = (char_t*)D_MALLOC(sizeof(char_t) * new_capacity);
    } else {
        string->data = (char_t*)D_REALLOC(string->data, sizeof(char_t) * new_capacity);
    }

    D_ASSERT(string->data != NULL);
    string->capacity = new_capacity;
}

void dstring_reserve(DString* string, size_t n)
{
    D_ASSERT(string->data != NULL);
    size_t remaining = string->capacity - string->size;

    if (n > remaining) {
        dstring_realloc(string, string->capacity + n);
    }
}

int dstring_empty(DString* string)
{
    D_ASSERT(string->data != NULL);
    return string->size == 0;
}

void dstring_clear(DString* string)
{
    D_ASSERT(string->data != NULL);
    memset(string->data, ' ', string->capacity);
    string->size = 0;
}

void dstring_shrink_to_fit(DString* string)
{
    D_ASSERT(string->data != NULL);
    if (string->size < string->capacity) {
        dstring_realloc(string, string->size);
    }
}

const char_t* dstring_to_cstr(DString* string)
{
    D_ASSERT(string->data != NULL);
    D_ASSERT(string->data[string->size] == '\0');
    return (const char_t*)string->data;
}

DString dstring_substring(DString* string, size_t offset, size_t count)
{
    D_ASSERT(string->data != NULL);
    D_ASSERT(offset < string->size);
    DString substring = dstring_create_with_capacity(count);
    memcpy(substring.data, string->data + offset, count);
    substring.size = count;
    substring.data[substring.size] = '\0';
    return substring;
}

void dstring_push(DString* string, char_t c)
{
    D_ASSERT(string->data != NULL);
    if (string->size + 1 >= string->capacity) {
        dstring_realloc(string, string->capacity * 2);
    }

    string->data[string->size] = c;
    string->size++;
    string->data[string->size] = '\0';
}

char_t dstring_pop(DString* string)
{
    D_ASSERT(string->data != NULL);
    D_ASSERT(string->size > 0);
    char_t c = string->data[string->size - 1];
    string->size--;
    string->data[string->size] = '\0';
    return c;
}

char_t dstring_front(DString* string)
{
    D_ASSERT(string->data != NULL);
    return string->data[0];
}

char_t dstring_back(DString* string)
{
    D_ASSERT(string->data != NULL);
    return string->data[string->size - 1];
}

void dstring_shift(DString* string, size_t offset, int32_t amount)
{
    D_ASSERT(string->data != NULL);
    D_ASSERT(offset < string->size);
    if (amount == 0) {
        return;
    }

    if (string->size + amount >= string->capacity) {
        dstring_realloc(string, string->capacity + amount);
    }

    char_t *dst = string->data + offset + amount;
    char_t *src = string->data + offset;
    size_t count = string->size - offset;
    memcpy(dst, src, count);

    // zero out unused data
    int shifted_right = amount > 1;
    if (shifted_right) {
        memset(string->data + offset, ' ', amount);
    }

    string->size += amount;
    string->data[string->size] = '\0';
}

void dstring_replace(DString* string, size_t offset, const char_t* old_val, const char_t* new_val)
{
    D_ASSERT(string->data != NULL);
    D_ASSERT(offset < string->size);
    size_t old_len = strlen(old_val);
    size_t new_len = strlen(new_val);
    
    if (old_len != new_len) {
        size_t diff = new_len - old_len;
        dstring_shift(string, offset + old_len, diff);
    }

    for (size_t i = 0; i < new_len; i++) {
        string->data[offset + i] = new_val[i];
    }
}

size_t dstring_find(DString* string, char_t c)
{
    D_ASSERT(string->data != NULL);

    for (size_t i = 0; i < string->size; i++) {
        if (string->data[i] == c) {
            return i;
        }
    }

    return DYN_STRING_NPOS;
}

size_t dstring_reverse_find(DString* string, char_t c)
{
    D_ASSERT(string->data != NULL);

    for (size_t i = string->size - 1; i >= 0; i--) {
        if (string->data[i] == c) {
            return i;
        }
    }

    return DYN_STRING_NPOS;
}

int dstring_cmp(DString* lhs, DString* rhs)
{
    D_ASSERT(lhs->data != NULL);
    D_ASSERT(rhs->data != NULL);
    if (lhs->size != rhs->size) {
        return 0;
    }

    return strcmp(lhs->data, rhs->data) == 0;
}

#endif