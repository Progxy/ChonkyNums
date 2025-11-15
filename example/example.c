#include <stdio.h>
#include <stdlib.h>

#define _CHONKY_NUMS_PRINTING_UTILS_
#define _CHONKY_NUMS_SPECIAL_TYPE_SUPPORT_
#define _CHONKY_NUMS_UTILS_IMPLEMENTATION_
#include "../chonky_nums.h"

int main(void) {
	u8 data_a[] = { 
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40,
	};
	
	u8 data_b[] = { 
		0x00, 0x00, 0xF0, 0x10, 0x00, 0x00, 0x00, 0x00
	};

	BigNum* a = alloc_chonky_num(data_a, ARR_SIZE(data_a), 0);
	if (a == NULL) return 1;
	
	BigNum* b = alloc_chonky_num(data_b, ARR_SIZE(data_b), 0);
	if (b == NULL) return 1;
	
	BigNum* c = chonky_pow(a, b);
	if (c == NULL) return 1;

	PRINT_CHONKY_NUM(*c);

	DEALLOC_CHONKY_NUMS(a, b, c);

	return 0;
}
