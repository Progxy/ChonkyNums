#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#define _CHONKY_NUMS_PRINTING_UTILS_
#define _CHONKY_NUMS_SPECIAL_TYPE_SUPPORT_
#define _CHONKY_NUMS_UTILS_IMPLEMENTATION_
#include "../chonky_nums.h"

// TODO: Implement Python based tests

int main(void) {
	u8 data_a[] = { 
		0x80, 0x90, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81,
	};
	
	u8 data_b[] = { 
		0x70, 0x90, 0x70, 0x70, 0x70, 0x70, 0x80, 0x70,
	};

	BigNum a = alloc_chonky_num(data_a, ARR_SIZE(data_a), 0);
	if (a.data == NULL) return 1;
	
	BigNum b = alloc_chonky_num(data_b, ARR_SIZE(data_b), 1);
	if (b.data == NULL) return 1;
	
	BigNum c = chonky_mul(a, b);
	if (c.data == NULL) return 1;

	PRINT_CHONKY_NUM(c);

	DEALLOC_CHONKY_NUMS(&a, &b, &c);

	return 0;
}
