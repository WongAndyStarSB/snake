#include "StringUtils.hpp"
#include <stdint.h>
#include <stddef.h>

namespace string_utils_ns {

int convertPtrToStringX64(const void* ptr, char* return_str) {
    uintptr_t ptr_val = (uintptr_t)ptr;
    return_str[0] = '0';
    return_str[1] = 'x';
    return_str[10] = '\0';
    for (size_t i = 9; i > 1; --i) {
        uint8_t remainder = ptr_val % 16;
        return_str[i] = (remainder < 10) ? ('0' + remainder) : ('a' + (remainder - 10));
        ptr_val /= 16;
    } 
    return 1;
}

}