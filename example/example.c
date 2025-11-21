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
	BigNum* a = alloc_chonky_num_from_hex_string("F2B9F3D7464C523FA37B5CE8DAFF2272BF29E5731C0FC57CB4A6E484085C1FA3F6955D6F9B7BD01278D4B8CFE59F97DF180FAE2E9F651BBDA3A2A3E0F677284E");
	if (a == NULL) return 1;
	
	BigNum* b = alloc_chonky_num_from_hex_string("FF");
	if (b == NULL) return 1;
	
	BigNum* c = alloc_chonky_num_from_hex_string("7FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFED");
	if (c == NULL) return 1;
	
	BigNum* res = chonky_pow_mod_mersenne(a, b, c);
	if (res == NULL) return 1;

	PRINT_CHONKY_NUM(a);
	PRINT_CHONKY_NUM(b);
	PRINT_CHONKY_NUM(c);
	PRINT_CHONKY_NUM(res);

	DEALLOC_CHONKY_NUMS(a, b, c, res);

	return 0;
}

int main(void) {
	if (example_a()) return 1;	
	if (example_b()) return 1;	
	if (example_c()) return 1;	
	return 0;
}

