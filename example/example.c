#include <stdio.h>
#include <stdlib.h>

#define _CHONKY_NUMS_PRINTING_UTILS_
#define _CHONKY_NUMS_SPECIAL_TYPE_SUPPORT_
#define _CHONKY_NUMS_UTILS_IMPLEMENTATION_
#include "../chonky_nums.h"

int example_a(void) {
	u8 data_a[] = { 
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40,
	};

	u8 data_b[] = { 
		0x00, 0x00, 0xF0, 0x10, 0x00, 0x00, 0x00, 0x00
	};

	BigNum* a = alloc_chonky_num(data_a, ARR_SIZE(data_a), 0);
	if (a == NULL) return 1;

	BigNum* b = alloc_chonky_num(data_b, ARR_SIZE(data_b), 1);
	if (b == NULL) return 1;
	
	BigNum* c = chonky_add(a, b);
	if (c == NULL) return 1;

	PRINT_CHONKY_NUM(a);
	PRINT_CHONKY_NUM(b);
	PRINT_CHONKY_NUM(c);

	DEALLOC_CHONKY_NUMS(a, b, c);
	
	return 0;
}

int example_b(void) {
	BigNum* a = alloc_chonky_num_from_hex_string("-0x00000000000000200000000000000040");
	if (a == NULL) return 1;
	
	BigNum* b = alloc_chonky_num_from_hex_string("0x0000F01000000000");
	if (b == NULL) return 1;
	
	BigNum* c = chonky_add(a, b);
	if (c == NULL) return 1;

	PRINT_CHONKY_NUM(a);
	PRINT_CHONKY_NUM(b);
	PRINT_CHONKY_NUM(c);

	DEALLOC_CHONKY_NUMS(a, b, c);

	return 0;
}

int example_c(void) {
	BigNum* a = alloc_chonky_num_from_string("-590295810358705651776");
	if (a == NULL) return 1;
	
	BigNum* b = alloc_chonky_num_from_string("263951510142976");
	if (b == NULL) return 1;
	
	BigNum* c = chonky_add(a, b);
	if (c == NULL) return 1;

	PRINT_CHONKY_NUM_DEC(a);
	PRINT_CHONKY_NUM_DEC(b);
	PRINT_CHONKY_NUM_DEC(c);

	DEALLOC_CHONKY_NUMS(a, b, c);

	return 0;
}

int main(void) {
	if (example_a()) return 1;	
	if (example_b()) return 1;	
	if (example_c()) return 1;	
	return 0;
}

