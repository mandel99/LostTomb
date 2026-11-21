#ifndef PHARAOH_CORE_MEMORY_STRINGS_H
#define PHARAOH_CORE_MEMORY_STRINGS_H

#include <stddef.h>

void *pharaoh_memory_set(void *destination, int value, size_t count);
char *pharaoh_string_copy_bounded(char *destination, const char *source,
    size_t count);
int pharaoh_string_compare(const char *left, const char *right);
size_t pharaoh_string_length(const char *text);

#endif
