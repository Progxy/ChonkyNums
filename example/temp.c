#include <stdio.h>
#include <stdlib.h>

#define _CHONKY_NUMS_PRINTING_UTILS_
#define _CHONKY_NUMS_SPECIAL_TYPE_SUPPORT_
#define _CHONKY_NUMS_UTILS_IMPLEMENTATION_
#include "../chonky_nums.h"

int main(void) {
    BigNum* a = alloc_chonky_num_from_string("12345678901234567890");
    BigNum* b = alloc_chonky_num_from_string("98765432109876543210");
    BigNum* result = chonky_add(a, b);
    PRINT_CHONKY_NUM(result); // Should output: 111111111011111111100
    PRINT_CHONKY_NUM_DEC(result); // Should output: 111111111011111111100
    DEALLOC_CHONKY_NUMS(a, b, result);
    return 0;
}
