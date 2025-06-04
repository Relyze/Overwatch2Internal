#include <stdio.h>
#include <stdarg.h>
#include <string.h>

#include "include/bdshemu/inc/bddisasm_types.h"

extern "C" {
    int nd_vsnprintf_s(char* buffer, ND_SIZET sizeOfBuffer, ND_SIZET count, const char* format, va_list argptr) {
        if (!buffer || !format || sizeOfBuffer == 0) {
            return -1;  // Handle invalid parameters
        }

        int written = vsnprintf(buffer, (count < sizeOfBuffer) ? count : sizeOfBuffer, format, argptr);

        return (written >= 0 && written < (int)sizeOfBuffer) ? written : -1;
    }

    char* nd_strcat_s(char* dst, ND_SIZET dst_size, const char* src) {
        if (!dst || !src || dst_size == 0) {
            return NULL;  // Handle invalid input
        }

        ND_SIZET dst_len = strnlen(dst, dst_size);
        ND_SIZET src_len = strnlen(src, dst_size);

        if (dst_len + src_len >= dst_size) {
            return NULL;  // Prevent buffer overflow
        }

        return strncat(dst, src, dst_size - dst_len - 1);
    }

    void* nd_memset(void* s, int c, ND_SIZET n) {
        return memset(s, c, n);
    }
}