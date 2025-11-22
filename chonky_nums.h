/*
 * Copyright (C) 2025 TheProgxy <theprogxy@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef _CHONKY_NUMS_H_
#define _CHONKY_NUMS_H_

#include <immintrin.h>

#define EXPORT_FUNCTION extern
#define EXPORT_ENUM
#define EXPORT_STRUCTURE

#ifndef NO_INLINE
	#define NO_INLINE __attribute__((__noinline__))
#endif //NO_INLINE

#ifndef PACKED_STRUCT
	#define PACKED_STRUCT __attribute__((packed))
#endif //PACKED_STRUCT

#ifndef UNUSED_FUNCTION
	#define UNUSED_FUNCTION __attribute__((unused))
#endif //UNUSED_FUNCTION

#ifndef TRUE
	#define FALSE 0
	#define TRUE  1
#endif //TRUE

#ifdef _CHONKY_NUMS_SPECIAL_TYPE_SUPPORT_

#define STATIC_ASSERT _Static_assert

typedef unsigned char bool;

typedef unsigned char      u8;
typedef unsigned short     u16;
typedef unsigned int       u32;
typedef unsigned long long u64;

STATIC_ASSERT(sizeof(u8)  == 1, "u8  must be 1 byte");
STATIC_ASSERT(sizeof(u16) == 2, "u16 must be 2 bytes");
STATIC_ASSERT(sizeof(u32) == 4, "u32 must be 4 bytes");
STATIC_ASSERT(sizeof(u64) == 8, "u64 must be 8 bytes");

typedef char      s8;
typedef short     s16;
typedef int       s32;
typedef long long s64;

STATIC_ASSERT(sizeof(s8)  == 1, "s8  must be 1 byte");
STATIC_ASSERT(sizeof(s16) == 2, "s16 must be 2 bytes");
STATIC_ASSERT(sizeof(s32) == 4, "s32 must be 4 bytes");
STATIC_ASSERT(sizeof(s64) == 8, "s64 must be 8 bytes");

#ifdef __SIZEOF_INT128__
	__extension__ typedef unsigned __int128 u128;
	__extension__ typedef          __int128 s128;

	STATIC_ASSERT(sizeof(u128) == 16, "u128 must be 16 bytes");
	STATIC_ASSERT(sizeof(s128) == 16, "s128 must be 16 bytes");
#endif //__SIZEOF_INT128__

#endif //_CHONKY_NUMS_SPECIAL_TYPE_SUPPORT_

// -------------------------------
// Printing Macros
// -------------------------------
#ifdef _CHONKY_NUMS_PRINTING_UTILS_
	#define RED           "\033[31m"
	#define GREEN         "\033[32m"
	#define BLUE          "\033[34m"
	#define PURPLE        "\033[35m"
	#define CYAN          "\033[36m"
	#define BRIGHT_YELLOW "\033[38;5;214m"    
	#define RESET_COLOR   "\033[0m"

	#define WARNING_COLOR BRIGHT_YELLOW
	#define ERROR_COLOR   RED
	#define DEBUG_COLOR   PURPLE
	#define TODO_COLOR    CYAN
	#define INFO_COLOR    BLUE

	#define COLOR_STR(str, COLOR) COLOR str RESET_COLOR

	#define ERROR_LOG(format, error_str, ...) printf(COLOR_STR("ERROR:%s:" __FILE__ ":%u: ", ERROR_COLOR) format "\n", error_str, __LINE__, ##__VA_ARGS__)
	#define WARNING_LOG(format, ...)          printf(COLOR_STR("WARNING:" __FILE__ ":%u: ", WARNING_COLOR) format "\n", __LINE__, ##__VA_ARGS__)
	#define INFO_LOG(format, ...)             printf(COLOR_STR("INFO:" __FILE__ ":%u: ", INFO_COLOR) format "\n", __LINE__, ##__VA_ARGS__)
	#ifdef _DEBUG
		#define DEBUG_LOG(format, ...)            printf(COLOR_STR("DEBUG:" __FILE__ ":%u: ", DEBUG_COLOR) format "\n", __LINE__, ##__VA_ARGS__)
	#else 
		#define DEBUG_LOG(format, ...)
	#endif //_DEBUG	
	
	#define TODO(format, ...)            printf(COLOR_STR("TODO:" __FILE__ ":%u: ", TODO_COLOR) COLOR_STR("function %s: ", PURPLE) format "\n", __LINE__, __func__, ##__VA_ARGS__)

#endif //_CHONKY_NUMS_PRINTING_UTILS_

#define SAFE_FREE(ptr) do { if ((ptr) != NULL) { free(ptr); (ptr) = NULL; } } while (0) 
#define GET_BIT(val, bit_pos) (((val) >> (bit_pos)) & 0x01)
#define CAST_PTR(ptr, type) ((type*) (ptr))
#define MAX(a, b) ((a) > (b) ? (a) : (b)) 
#define MIN(a, b) ((a) < (b) ? (a) : (b)) 
#define ARR_SIZE(arr) (sizeof(arr) / sizeof(*(arr)))

#ifdef _CHONKY_NUMS_UTILS_IMPLEMENTATION_

#include <stdarg.h>

// TODO: This has to go, as we want to also support kernel modules
#define CHONKY_ASSERT(condition) chonky_assert(condition, #condition, __FILE__, __LINE__, __func__)
static void chonky_assert(bool condition, const char* condition_str, const char* file, const int line, const char* func) {
	if (condition) return;
	printf(COLOR_STR("ASSERT::%s:%u: ", ERROR_COLOR) "Failed to assert condition " COLOR_STR("'%s'", BLUE) " in function " COLOR_STR("'%s'", PURPLE) "\n", file, line, condition_str, func);
	abort();
	return;
}

static u8 bit_size(u8 val) {
	u8 size = 8;
	for (s8 i = size - 1; i >= 0; --i) {
		if (GET_BIT(val, i)) break;
		size--;
	}
	return size;
}

static size_t str_len(const char* str) {
    if (str == NULL) return 0;
    const char* str_c = str;
	while (*str++);
    return (size_t) (str - str_c - 1);
}

static void* mem_cpy(void* dest, const void* src, size_t size) {
	if (dest == NULL || src == NULL) return NULL;
	for (size_t i = 0; i < size; ++i) CAST_PTR(dest, u8)[i] = CAST_PTR(src, u8)[i];
	return dest;
}

#define mem_set(ptr, value, size)    mem_set_var(ptr, value, size, sizeof(u8))
#define mem_set_32(ptr, value, size) mem_set_var(ptr, value, size, sizeof(u32))
#define mem_set_64(ptr, value, size) mem_set_var(ptr, value, size, sizeof(u64))
static void mem_set_var(void* ptr, int value, size_t size, size_t val_size) {
	if (ptr == NULL) return;
	for (size_t i = 0; i < size; ++i) CAST_PTR(ptr, u8)[i] = CAST_PTR(&value, u8)[i % val_size]; 
	return;
}

static char* reverse_str(char* str) {
    int len = str_len(str);
    for (int i = 0; i < (len / 2); ++i) {
        char temp = str[i];
        str[i] = str[len - i - 1];
        str[len - i - 1] = temp;
    }
    return str;
}

#endif // _CHONKY_NUMS_UTILS_IMPLEMENTATION_

// TODO: Check memory deallocations on error paths.
// TODO: Check for error handling paths.
// TODO: Refactor a bit and clean.
/// -----------------------------------------
///  BigNum Structure Manipulation Functions
/// -----------------------------------------
EXPORT_STRUCTURE typedef struct BigNum {
	union {
		u8* data;
		u64* data_64;
	};
	u64 size;
	u8 sign;
} BigNum;

static inline u64 align_64(u64 val) {
	if (val == 0) return 8;
	return val + (val % 8 ? (8 - (val % 8)) : 0);
}

EXPORT_FUNCTION BigNum* alloc_chonky_num(const u8* data, const u64 size, bool sign) {
	BigNum* num = calloc(1, sizeof(BigNum));
	if (num == NULL) {
		WARNING_LOG("Failed to allocate BigNum.");
		return NULL;
	}

	num -> sign = sign;
	num -> size = align_64(size);

	num -> data = (u8*) calloc(num -> size, sizeof(u8));
	if (num -> data == NULL) {
		free(num);
		WARNING_LOG("Failed to allocate data buffer.");
		return NULL;
	}
	
	if (data != NULL) mem_cpy(num -> data, data, size);

	return num;
}

static BigNum* dup_chonky_num(const BigNum* num) {
	BigNum* duped = alloc_chonky_num(num -> data, num -> size, num -> sign);
	return duped;
}

static BigNum* copy_chonky_num(BigNum* num, const BigNum* src) {
	num -> data = (u8*) realloc(num -> data, src -> size * sizeof(u8));
	if (num -> data == NULL) {
		WARNING_LOG("Failed to resize data buffer.");
		return NULL;
	}

	mem_cpy(num -> data, src -> data, num -> size);
	
	return num;
}

static u64 chonky_real_size(const BigNum* num) {
	if (num -> data == NULL) return 0;
	u64 size = num -> size;
	for (s64 i = num -> size - 1; i >= 0; --i) {
		if ((num -> data)[i]) break;
		size--;
	}
	return size;
}

static u64 chonky_real_size_64(const BigNum* num) {
	if (num -> data == NULL) return 0;
	u64 size = num -> size / 8;
	for (s64 i = (num -> size / 8) - 1; i >= 0; --i) {
		if ((num -> data_64)[i]) break;
		size--;
	}
	return size;
}

static u64 chonky_bit_size(const BigNum* num) {
	if (num -> data == NULL) return 0;
	
	u64 size = num -> size;
	for (s64 i = num -> size - 1; i >= 0; --i) {
		if ((num -> data)[i]) break;
		size--;
	}
	
	s8 bit_cnt = 0;
	for (bit_cnt = 7; bit_cnt >= 0; --bit_cnt) {
		if (GET_BIT((num -> data)[size - 1], bit_cnt)) break;
	}
	
	size = size * 8 - (7 - bit_cnt);
	
	return size;
}

static int chonky_resize(BigNum* num, u64 new_size) {
	if (new_size == 0) new_size = align_64(chonky_real_size(num));
	else new_size = align_64(new_size);

	num -> data = (u8*) realloc(num -> data, new_size * sizeof(u8));
	if (num -> data == NULL) {
		WARNING_LOG("Failed to resize data buffer, from %llu to %llu.", num -> size, new_size);
		free(num);
		return -1;
	}

	num -> size = new_size;
	
	return 0;
}

#define DEALLOC_CHONKY_NUMS(...) dealloc_chonky_nums((sizeof((BigNum*[]){__VA_ARGS__}) / sizeof(BigNum*)),  __VA_ARGS__)
void dealloc_chonky_nums(int len, ...) {
	va_list args;
	va_start(args, len);

	for (int i = 0; i < len; ++i) {
		BigNum* num = va_arg(args, BigNum*);
		free(num -> data);
		num -> data = NULL;
		free(num);
    }
    
	va_end(args);
	
	return;
}

EXPORT_FUNCTION void dealloc_chonky_num(BigNum* num) {
	free(num -> data);
	num -> data = NULL;
	free(num);
	return;
}

static int chonky_shift_dec(BigNum* num, u64* temp) {
	const u64 num_size = MIN(num -> size / 8, align_64(chonky_real_size(num)) / 8 + 1);

	mem_cpy(temp, num -> data, num -> size);
	mem_set(num -> data, 0, num -> size);

	for (u64 i = 0; i < num_size; ++i) {
		*((u128*) (num -> data_64 + i)) += ((u128) temp[i]) * 10;
	}

	return 0;
}

static void chonky_add_decimal(BigNum* num, const u8 dec) {
	u64 carry = _addcarry_u64(0, *(num -> data_64), dec, num -> data_64);
	const u64 num_size = MIN(num -> size / 8, align_64(chonky_real_size(num)) / 8 + 1);
	
	for (u64 i = 1; carry && i < num_size; ++i) {
		carry = _addcarry_u64(carry, (num -> data_64)[i], 0, num -> data_64 + i);
	}
	
	return;
}

#define IS_A_DEC_DIGIT(c) (((c) >= '0') && ((c) <= '9'))
EXPORT_FUNCTION BigNum* alloc_chonky_num_from_string(const char* data_str) {
	const u64 data_str_len = str_len(data_str);
	if (data_str == NULL || data_str_len == 0) {
		WARNING_LOG("Invalid parameters.");
		return NULL;
	}
	
	BigNum* num = calloc(1, sizeof(BigNum));
	if (num == NULL) {
		WARNING_LOG("Failed to allocate BigNum.");
		return NULL;
	}

	num -> sign = (data_str[0] == '-');
	num -> size = align_64(data_str_len / 2 + 1);

	num -> data = (u8*) calloc(num -> size, sizeof(u8));
	if (num -> data == NULL) {
		free(num);
		WARNING_LOG("Failed to allocate data buffer.");
		return NULL;
	}
	
	u64* temp = calloc(num -> size, sizeof(u8));
	if (temp == NULL) {
		dealloc_chonky_num(num);
		WARNING_LOG("Failed to allocate temp buffer.");
		return NULL;
	}
	
	for (u64 i = (*data_str == '-'); i < data_str_len; ++i) {
		if (!IS_A_DEC_DIGIT(data_str[i])) {
			SAFE_FREE(temp);
			dealloc_chonky_num(num);
			WARNING_LOG("'%c': is not a valid digit.", data_str[i]);
			return NULL;
		}
		
		chonky_shift_dec(num, temp);
		chonky_add_decimal(num, data_str[i] - '0');
	}

	SAFE_FREE(temp);

	if (chonky_resize(num, 0)) return NULL;

	return num;
}

static inline u8 char_to_hex(char c) {
	u8 val = 0;
	
	if      (c >= '0' && c <= '9') val = c - '0';
	else if (c >= 'a' && c <= 'f') val = c - 'a' + 10;
	else                           val = c - 'A' + 10;
	
	return val;
}

#define IS_A_HEX_DIGIT(c) ((((c) >= '0') && ((c) <= '9')) || (((c) >= 'a') && ((c) <= 'f')) || (((c) >= 'A') && ((c) <= 'F')))
EXPORT_FUNCTION BigNum* alloc_chonky_num_from_hex_string(const char* data_str) {
	const u64 data_str_len = str_len(data_str);
	if (data_str == NULL || data_str_len == 0) {
		WARNING_LOG("Invalid parameters.");
		return NULL;
	}
	
	BigNum* num = calloc(1, sizeof(BigNum));
	if (num == NULL) {
		WARNING_LOG("Failed to allocate BigNum.");
		return NULL;
	}

	char* hex_str = (char*) data_str + (*data_str == '-');
	u64 hex_str_len = str_len(hex_str);
	
	if (hex_str_len > 2 && *hex_str == '0' && hex_str[1] == 'x') hex_str += 2;
	hex_str_len = str_len(hex_str);
	
	num -> sign = (data_str[0] == '-');
	num -> size = align_64((hex_str_len - (hex_str_len % 2)) / 2 + (hex_str_len % 2));

	num -> data = (u8*) calloc(num -> size, sizeof(u8));
	if (num -> data == NULL) {
		free(num);
		WARNING_LOG("Failed to allocate data buffer.");
		return NULL;
	}

	for (s64 i = hex_str_len - 1, j = 0; i >= 0; i -= 2, ++j) {
		if (!IS_A_HEX_DIGIT(hex_str[i])) {
			dealloc_chonky_num(num);
			WARNING_LOG("'%c': is not a valid digit.", hex_str[i]);
			return NULL;
		} else if ((i - 1 >= 0) && !IS_A_HEX_DIGIT(hex_str[i - 1])) {
			dealloc_chonky_num(num);
			WARNING_LOG("'%c': is not a valid digit.", hex_str[i - 1]);
			return NULL;
		}

		u8 low = char_to_hex(hex_str[i]);
		u8 high = (i - 1 >= 0) ? char_to_hex(hex_str[i - 1]) : 0;
		(num -> data)[j] = low | (high << 4);
	}

	return num;
}

/// -------------------------------
///  Generic Operations Functions
/// -------------------------------
static BigNum* __chonky_rshift(BigNum* num, u64 bit_cnt) {
	const u64 byte_cnt = (bit_cnt / 8) + !!(bit_cnt % 8);
	if (byte_cnt > num -> size) {
		mem_set(num -> data, 0, num -> size);
		return num;
	}
	
	const u64 base_byte_cnt = (bit_cnt / 8);
	mem_cpy(num -> data, num -> data + base_byte_cnt, num -> size - base_byte_cnt);
	mem_set(num -> data + num -> size - base_byte_cnt, 0, base_byte_cnt);

	const u64 base_bits_rem = (bit_cnt % 8);
	const u64 num_cur_size = num -> size - base_byte_cnt;
	const u8 shift_mask = ((1 << base_bits_rem) - 1);

	u8 prev_rem = 0;
	for (s64 i = num_cur_size - 1; i >= 0; --i) {
		u8 cur_rem = ((num -> data)[i] & shift_mask) << (8 - base_bits_rem);
		(num -> data)[i] = prev_rem | ((num -> data)[i] >> base_bits_rem);
		prev_rem = cur_rem;
	}

	return num;
}

static bool chonky_is_gt(const BigNum* a, const BigNum* b) {
	if (a == NULL || b == NULL) {
		WARNING_LOG("Parameters must be not null.");
		return FALSE;
	}

	u64 a_size = chonky_real_size(a);
	u64 b_size = chonky_real_size(b);
	
	if (a_size > b_size) return TRUE;
	else if (a_size < b_size) return FALSE;

	for (s64 i = (align_64(a_size) / 8) - 1; i >= 0; --i) {
		if ((a -> data_64)[i] > (b -> data_64)[i]) return TRUE;
		else if ((b -> data_64)[i] > (a -> data_64)[i]) return FALSE;
	}
	
	return FALSE;
}

static bool is_chonky_zero(BigNum* num) {
	if (num == NULL) {
		WARNING_LOG("Parameters must be not null.");
		return FALSE;
	}

	for (u64 i = 0; i < num -> size / 8; ++i) {
		if ((num -> data_64)[i]) return FALSE;
	}
	
	return TRUE;
}

static u8 chonky_dec_divmod(BigNum* num) {
	u32 rem = 0;
	const u64 num_size = chonky_real_size(num);

	for (s64 i = num_size - 1; i >= 0; --i) {
        u32 cur = (rem << 8) + (num -> data)[i];
        (num -> data)[i] = cur / 10;
        rem = cur % 10;
	}

	return rem;
}

#define PRINT_CHONKY_NUM(num)     print_chonky_num(#num, num, TRUE)
#define PRINT_CHONKY_NUM_DEC(num) print_chonky_num(#num, num, FALSE)
EXPORT_FUNCTION void print_chonky_num(char* name, BigNum* num, bool use_hex) {
	const u64 real_size = chonky_real_size(num);
	
	printf("%s: %s", name, num -> sign ? "-" : "");

	if (use_hex) {
		for (s64 i = real_size - 1; i >= 0; --i) {
			printf("%02X", (num -> data)[i]);
		}
	} else {
		BigNum* num_dp = dup_chonky_num(num);
		if (num_dp == NULL) {
			WARNING_LOG("Failed to dupe the big num.");
			return;
		}

		char* buf = calloc(num_dp -> size * 3, sizeof(u8));
		if (num_dp == NULL) {
			dealloc_chonky_num(num_dp);
			WARNING_LOG("Failed to alloc the buffer.");
			return;
		}

		for (u64 i = 0; !is_chonky_zero(num_dp); ++i) {
			buf[i] = chonky_dec_divmod(num_dp) + '0';
		}

		reverse_str(buf);

		dealloc_chonky_num(num_dp);

		printf("%s", buf);
		SAFE_FREE(buf);
	}

	printf(" (size: %llu, real size: %llu)\n", num -> size, real_size);

	return;
}

/// -------------------------------
///  Internal Operations Functions
/// -------------------------------
static BigNum* __chonky_add(BigNum* res, const BigNum* a, const BigNum* b) {
	const u64 a_size = chonky_real_size_64(a);
	const u64 b_size = chonky_real_size_64(b);
	const u64 size = MIN(res -> size / 8, MAX(a_size, b_size) + 1); 

	u64 carry = 0;
	for (u64 i = 0; i < size; ++i) {
		u64* acc = res -> data_64 + i;
		const u64 a_val = (i < a_size) ? (a -> data_64)[i] : 0;
		const u64 b_val = (i < b_size) ? (b -> data_64)[i] : 0;
		carry = _addcarry_u64(carry, a_val, b_val, acc);
	}

	return res;
}

static BigNum* __chonky_sub(BigNum* res, const BigNum* a, const BigNum* b) {
	const u64 a_size = chonky_real_size_64(a);
	const u64 b_size = chonky_real_size_64(b);
	const u64 size = MIN(res -> size / 8, MAX(a_size, b_size) + 1); 
	
	u64 carry = 0;
	for (u64 i = 0; i < size; ++i) {
		u64* acc = res -> data_64 + i;
		const u64 a_val = (i < a_size) ? (a -> data_64)[i] : 0;
		const u64 b_val = (i < b_size) ? (b -> data_64)[i] : 0;
		carry = _subborrow_u64(carry, a_val, b_val, acc);
	}
	
	if (res -> sign) {
		for (u64 i = 0; i < size; ++i) {
			u64* acc = res -> data_64 + i;
			*acc = ~*acc;
		   	if (i == 0) (*acc)++;
		}
	}

	return res;
}

static BigNum* __chonky_mul_s(BigNum* res, const BigNum* a, const BigNum* b) {
	u64 a_size = align_64(chonky_real_size(a));
	u64 b_size = align_64(chonky_real_size(b));
	CHONKY_ASSERT(res -> size > (a_size + b_size));
	
	BigNum* res_c = alloc_chonky_num(NULL, res -> size, 0);
	if (res_c == NULL) return NULL;

	for (u64 i = 0; i < (a_size / 4); ++i) {
		for (u64 j = 0; j < (b_size / 4); ++j) {
			*((u128*) (res_c -> data + (i + j) * 4)) += (u64) ((u32*) a -> data)[i] * ((u32*) b -> data)[j];
		}
	}
	
	mem_cpy(res -> data, res_c -> data, res -> size);
	dealloc_chonky_num(res_c);

	return res;
}

/// TODO: Should maybe find a better solution?
static void __chonky_divstep(u64 size_diff, BigNum* a, const BigNum* b, u64 c) {
	const u32 _c[2] = { c & 0xFFFFFFFF, (c >> 32) & 0xFFFFFFFF };

	u64 i = 0;
	u64 carry[5] = {0};
	for (i = 0; i < align_64(chonky_real_size(b)) / 4; ++i) {
		for (u8 j = 0; j < 2; ++j) {
			u64 val = (u64) ((u32*) b -> data)[i] * _c[j % 2];
			carry[j + 3] = _subborrow_u64(carry[j], *((u64*) (a -> data + (i + j) * 4 + size_diff)), val, (u64*) (a -> data + (i + j) * 4 + size_diff));
		}
		carry[0] = carry[2], carry[2] = 0;
		carry[1] = carry[3], carry[3] = 0;
	}

	CHONKY_ASSERT(!carry[0] && !carry[1] && !carry[2] && !carry[3] && !carry[4]);

	return;
}

static BigNum* __chonky_div(BigNum* quotient, BigNum* remainder, const BigNum* a, const BigNum* b) {
	// NOTE: We do not support floating point division for now
	if (chonky_real_size(a) < chonky_real_size(b)) return quotient;
	
	BigNum* a_c = dup_chonky_num(a);
	if (a_c == NULL) return NULL;
	
	BigNum* b_c = dup_chonky_num(b);
	if (b_c == NULL) {
		dealloc_chonky_num(a_c);
		return NULL;
	}

	a_c -> sign = 0;
	b_c -> sign = 0;

	mem_set(quotient -> data, 0, quotient -> size);

	const u64 low_limit = chonky_real_size(b_c) - 1;
	u64 i = chonky_real_size(a_c) - 1;

	while (!chonky_is_gt(b_c, a_c)) {
		// Perform the division with the upper components
		const u8 additional = (low_limit >= 8) ? ((b_c -> data)[low_limit - 8]) && ((b_c -> data)[low_limit - 8] >= (a_c -> data)[i - 7]) : 0;
		
		u64 a_val = *((u64*) (a_c -> data + i - 7));
		u64 b_val = *((u64*) (b_c -> data + low_limit - 7)) + additional;
		
		if (i > low_limit && a_val < b_val) {
			b_val = (b_val >> 8) + 1;
			i--;
		}
		
		u64 q_hat = a_val / b_val;
		CHONKY_ASSERT(q_hat != 0);

		__chonky_divstep(i - low_limit, a_c, b_c, q_hat);
		
		*((u64*) (quotient -> data + i - low_limit)) += q_hat;
		i = chonky_real_size(a_c) - 1;
	}
	
	if (remainder != NULL) copy_chonky_num(remainder, a_c);

	DEALLOC_CHONKY_NUMS(a_c, b_c);

	return quotient;
}

static BigNum* __chonky_pow(BigNum* res, const BigNum* num, const BigNum* exp) {
    BigNum* temp = dup_chonky_num(res);
	if (temp == NULL) return NULL;
	
	BigNum* temp_base = alloc_chonky_num(NULL, res -> size, num -> sign);
	if (temp_base == NULL) {
		dealloc_chonky_num(temp);
		return NULL;
	}	

	BigNum* base = alloc_chonky_num(NULL, res -> size, num -> sign);
	if (base == NULL) {
		DEALLOC_CHONKY_NUMS(temp, temp_base);
		return NULL;
	}	
	
	mem_set(res -> data, 0, res -> size);
	*(res -> data) = 1;
	mem_cpy(base -> data, num -> data, num -> size);

	const u64 step_cnt = chonky_real_size(exp);
	for (u64 i = 0; i < step_cnt; ++i) {
		const u8 bit_s = (i < step_cnt - 1) ? 8 : bit_size((exp -> data)[i]);
		for (u8 j = 0; j < bit_s; ++j) {
			if (GET_BIT((exp -> data)[i], j) == 1) {
				__chonky_mul_s(temp, res, base);
				mem_cpy(res -> data, temp -> data, res -> size);
			}

			if (i == step_cnt - 1 && j == bit_s - 1) break;

			__chonky_mul_s(temp_base, base, base);
			mem_cpy(base -> data, temp_base -> data, base -> size);
		}
	}
	
	DEALLOC_CHONKY_NUMS(base, temp, temp_base);

	return res;
}

static BigNum* __chonky_mod(BigNum* res, const BigNum* num, const BigNum* base) {
	if (chonky_is_gt(base, num)) {
		mem_cpy(res -> data, num -> data, num -> size);
		return res;
	} 

	BigNum* quotient = alloc_chonky_num(NULL, align_64(MAX(num -> size, base -> size)), 0);
	if (quotient == NULL) return NULL;
	
	__chonky_div(quotient, res, num, base);

	DEALLOC_CHONKY_NUMS(quotient);
	
	return res;
}

static BigNum* __chonky_mask(BigNum* res, BigNum* num, const u64 bit_cnt) {
	const u64 bits_rem = bit_cnt % 8;
	const u64 byte_cnt = (bit_cnt - bits_rem) / 8;
	
	mem_set(res -> data, 0, res -> size);

	for (u64 i = 0; i < byte_cnt; ++i) {
		(res -> data)[i] = (num -> data)[i] & 0xFF;
	}

	if (bits_rem) {
		(res -> data)[byte_cnt] = (num -> data)[byte_cnt] & ((1 << bits_rem) - 1);
	}

	return res;
}

static BigNum* get_mersenne_factor(const BigNum* num, BigNum* mersenne_c) {
	const u64 num_bit_size = chonky_bit_size(num);
	
	BigNum* temp = alloc_chonky_num(NULL, (num_bit_size / 8) + 8, 0);
	if (temp == NULL) return NULL;

	const u64 num_bit_size_rem = num_bit_size % 8;
	(temp -> data)[(num_bit_size - num_bit_size_rem) / 8] = 1 << num_bit_size_rem;
		
	__chonky_sub(mersenne_c, temp, num);

	dealloc_chonky_num(temp);

	return mersenne_c;
}

static BigNum* __chonky_mod_mersenne(BigNum* res, const BigNum* num, const BigNum* base) {
	if (chonky_is_gt(base, num)) {
		mem_cpy(res -> data, num -> data, base -> size);
		return res;
	} else if (chonky_real_size_64(base) == chonky_real_size_64(num)) {
		__chonky_mod(res, num, base);
		return res;	
	}
	
	if (get_mersenne_factor(base, res) == NULL) return NULL;
	
	const u64 base_size = chonky_real_size_64(base);
	BigNum* temp_res = alloc_chonky_num(NULL, num -> size + (num -> size - base_size * 8), 0);
	if (temp_res == NULL) return NULL;
	
	mem_cpy(temp_res -> data, num -> data, num -> size);

	const u64 base_bit_cnt = chonky_bit_size(base);
	BigNum* low = alloc_chonky_num(NULL, base_size * 8, 0);
	if (low == NULL) {
		dealloc_chonky_num(temp_res);
		return NULL;
	}

	while (chonky_is_gt(temp_res, base)) {
		__chonky_mask(low, temp_res, base_bit_cnt);
		__chonky_rshift(temp_res, base_bit_cnt);
		
		if (!is_chonky_zero(temp_res)) {
			__chonky_mul_s(temp_res, temp_res, res);
			__chonky_add(temp_res, temp_res, low);
		} else {
			__chonky_sub(temp_res, low, base);
		}
	}

	mem_cpy(res -> data, temp_res -> data, res -> size);

	DEALLOC_CHONKY_NUMS(low, temp_res);

	return res;
}

// TODO: Error Handling missing in this function
// TODO: There is probably also some method to reduce the exponent
static BigNum* __chonky_pow_mod(BigNum* res, const BigNum* num, const BigNum* exp, const BigNum* mod_base) {
 	BigNum* temp = alloc_chonky_num(NULL, res -> size * 2 + 8, res -> sign);
	if (temp == NULL) return NULL;

	BigNum* temp_base = alloc_chonky_num(NULL, res -> size * 2 + 8, num -> sign);
	if (temp_base == NULL) {
		dealloc_chonky_num(temp);
		return NULL;
	}	

	BigNum* base = alloc_chonky_num(NULL, res -> size, num -> sign);
	if (base == NULL) {
		DEALLOC_CHONKY_NUMS(temp, temp_base);
		return NULL;
	}	
	
	mem_set(res -> data, 0, res -> size);
	*(res -> data) = 1;

	if (chonky_is_gt(num, mod_base)) __chonky_mod(base, num, mod_base);
	else mem_cpy(base -> data, num -> data, num -> size);

	for (u64 i = 0; i < chonky_real_size(exp); ++i) {
		const u8 bit_s = (i < chonky_real_size(exp) - 1) ? 8 : bit_size((exp -> data)[i]);
		for (u8 j = 0; j < bit_s; ++j) {
			if (GET_BIT((exp -> data)[i], j) == 1) {
				__chonky_mul_s(temp, res, base);
				if (chonky_is_gt(temp, mod_base)) __chonky_mod(res, temp, mod_base);
				else mem_cpy(res -> data, temp -> data, res -> size);
			}

			__chonky_mul_s(temp_base, base, base);
			if (chonky_is_gt(temp_base, mod_base)) __chonky_mod(base, temp_base, mod_base);
			else mem_cpy(base -> data, temp_base -> data, base -> size);
		}
	}
	
	DEALLOC_CHONKY_NUMS(base, temp, temp_base);

	return res;
}

// TODO: Error Handling missing in this function
// TODO: There is probably also some method to reduce the exponent
static BigNum* __chonky_pow_mod_mersenne(BigNum* res, const BigNum* num, const BigNum* exp, const BigNum* mod_base) {
 	BigNum* temp = alloc_chonky_num(NULL, res -> size * 2 + 8, res -> sign);
	if (temp == NULL) return NULL;

	BigNum* temp_base = alloc_chonky_num(NULL, res -> size * 2 + 8, num -> sign);
	if (temp_base == NULL) {
		dealloc_chonky_num(temp);
		return NULL;
	}	

	BigNum* base = alloc_chonky_num(NULL, res -> size, num -> sign);
	if (base == NULL) {
		DEALLOC_CHONKY_NUMS(temp, temp_base);
		return NULL;
	}	
	
	mem_set(res -> data, 0, res -> size);
	*(res -> data) = 1;
	
	if (chonky_is_gt(num, mod_base)) __chonky_mod_mersenne(base, num, mod_base);
	else mem_cpy(base -> data, num -> data, num -> size);

	for (u64 i = 0; i < chonky_real_size(exp); ++i) {
		const u8 bit_s = (i < chonky_real_size(exp) - 1) ? 8 : bit_size((exp -> data)[i]);
		for (u8 j = 0; j < bit_s; ++j) {
			if (GET_BIT((exp -> data)[i], j) == 1) {
				__chonky_mul_s(temp, res, base);
				if (chonky_is_gt(temp, mod_base)) __chonky_mod_mersenne(res, temp, mod_base);
				else mem_cpy(res -> data, temp -> data, res -> size);
			}

			__chonky_mul_s(temp_base, base, base);
			if (chonky_is_gt(temp_base, mod_base)) __chonky_mod_mersenne(base, temp_base, mod_base);
			else mem_cpy(base -> data, temp_base -> data, base -> size);
		}
	}
	
	DEALLOC_CHONKY_NUMS(base, temp, temp_base);

	return res;
}


EXPORT_FUNCTION BigNum* chonky_add(const BigNum* a, const BigNum* b) {
	if (a == NULL || b == NULL) {
		WARNING_LOG("Parameters must be not null.");
		return NULL;
	}

	BigNum* res = alloc_chonky_num(NULL, align_64(MAX(a -> size, b -> size) + 1), 0);
	if (res == NULL) return NULL;

	res -> sign = (a -> sign && b -> sign)          || 
		          (chonky_is_gt(a, b) && a -> sign) || 
			      (chonky_is_gt(b, a) && b -> sign);
	
	if ((a -> sign && !b -> sign) || (!a -> sign && b -> sign)) {
		const BigNum* minuend = a -> sign ? b : a;
		const BigNum* subtraend = a -> sign ? a : b;
		res = __chonky_sub(res, minuend, subtraend);
	} else res = __chonky_add(res, a, b);
	   	
	return res;
}

EXPORT_FUNCTION BigNum* chonky_sub(const BigNum* a, const BigNum* b) {
	BigNum* minuend = dup_chonky_num(b);
	if (minuend == NULL) return NULL;
	minuend -> sign = !(b -> sign);
	BigNum* res = chonky_add(a, minuend);
	dealloc_chonky_num(minuend);
	return res;
}

EXPORT_FUNCTION BigNum* chonky_mul(const BigNum* a, const BigNum* b) {
	if (a == NULL || b == NULL) {
		WARNING_LOG("Parameters must be not null.");
		return NULL;
	}

	BigNum* res = alloc_chonky_num(NULL, align_64(a -> size + b -> size) + 8, 0);
	if (res == NULL) return NULL;
	
	res -> sign = a -> sign | b -> sign;
	res = __chonky_mul_s(res, a, b);

	return res;
}

EXPORT_FUNCTION BigNum* chonky_div(const BigNum* a, const BigNum* b) {
	if (a == NULL || b == NULL) {
		WARNING_LOG("Parameters must be not null.");
		return NULL;
	}

	BigNum* res = alloc_chonky_num(NULL, align_64(MAX(a -> size, b -> size)), 0);
	if (res == NULL) return NULL;

	res -> sign = a -> sign | b -> sign;
	
	res = __chonky_div(res, NULL, a, b);
	
	if (chonky_resize(res, 0)) return NULL;

	return res;
}

static u64 __chonky_spow(u64 base, u64 exp) {
	u64 result = 1;
	for (u64 i = 0; i < exp; ++i) result *= base;
	return result;
}

EXPORT_FUNCTION BigNum* chonky_pow(const BigNum* num, const BigNum* exp) {
	if (num == NULL || exp == NULL) {
		WARNING_LOG("Parameters must be not null.");
		return NULL;
	} else if (chonky_real_size(exp) > 4) {
		WARNING_LOG("Pure exponentiation, does not allow for exponent with more than four bytes (%llu).", chonky_real_size(exp));
		return NULL;
	}

	const u64 size = chonky_real_size(num) * __chonky_spow(2, chonky_real_size(exp) * 8);
	BigNum* res = alloc_chonky_num(NULL, align_64(size), 0);
	if (res == NULL) return NULL;

	res -> sign = num -> sign * ((exp -> data)[0] & 0x01);
	res = __chonky_pow(res, num, exp);
	
	if (chonky_resize(res, 0)) return NULL;

	return res;
}

EXPORT_FUNCTION BigNum* chonky_mod(const BigNum* num, const BigNum* mod) {
	if (num == NULL || mod == NULL) {
		WARNING_LOG("Parameters must be not null.");
		return NULL;
	}

	const u64 size = chonky_real_size(mod);
	BigNum* res = alloc_chonky_num(NULL, align_64(size), 0);
	if (res == NULL) return NULL;

	__chonky_mod(res, num, mod);
	
	if (chonky_resize(res, 0)) return NULL;

	return res;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * NOTE:                                                   * 
 * We do not check if the mod given is a Mersenne prime,   *
 * as that's the job of the caller                         *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
EXPORT_FUNCTION BigNum* chonky_mod_mersenne(const BigNum* num, const BigNum* mod) {
	if (num == NULL || mod == NULL) {
		WARNING_LOG("Parameters must be not null.");
		return NULL;
	}

	const u64 size = chonky_real_size(mod);
	BigNum* res = alloc_chonky_num(NULL, align_64(size), 0);
	if (res == NULL) return NULL;

	__chonky_mod_mersenne(res, num, mod);
	
	if (chonky_resize(res, 0)) return NULL;

	return res;
}

EXPORT_FUNCTION BigNum* chonky_pow_mod(const BigNum* num, const BigNum* exp, const BigNum* mod) {
	if (num == NULL || exp == NULL || mod == NULL) {
		WARNING_LOG("Parameters must be not null.");
		return NULL;
	}

	const u64 size = chonky_real_size(mod);
	BigNum* res = alloc_chonky_num(NULL, align_64(size), 0);
	if (res == NULL) return NULL;

	__chonky_pow_mod(res, num, exp, mod);
	
	if (chonky_resize(res, 0)) return NULL;

	return res;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * NOTE:                                                   * 
 * We do not check if the mod given is a Mersenne prime,   *
 * as that's the job of the caller                         *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
EXPORT_FUNCTION BigNum* chonky_pow_mod_mersenne(const BigNum* num, const BigNum* exp, const BigNum* mod) {
	if (num == NULL || mod == NULL) {
		WARNING_LOG("Parameters must be not null.");
		return NULL;
	}

	const u64 size = chonky_real_size(mod);
	BigNum* res = alloc_chonky_num(NULL, align_64(size), 0);
	if (res == NULL) return NULL;

	__chonky_pow_mod_mersenne(res, num, exp, mod);
	
	if (chonky_resize(res, 0)) return NULL;

	return res;
}

#endif //_CHONKY_NUMS_H_

