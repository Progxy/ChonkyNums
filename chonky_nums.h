#ifndef _CHONKY_NUMS_H_
#define _CHONKY_NUMS_H_

#include <immintrin.h>

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

#define STATIC_ASSERT          _Static_assert

typedef unsigned char      u8;
typedef unsigned short     u16;
typedef unsigned int       u32;
typedef unsigned long long u64;
typedef unsigned __int128  u128;

typedef char      s8;
typedef short     s16;
typedef int       s32;
typedef long long s64;
typedef __int128  s128;

STATIC_ASSERT(sizeof(u8)   == 1,   "u8   must be 1  byte");
STATIC_ASSERT(sizeof(u16)  == 2,   "u16  must be 2  bytes");
STATIC_ASSERT(sizeof(u32)  == 4,   "u32  must be 4  bytes");
STATIC_ASSERT(sizeof(u64)  == 8,   "u64  must be 8  bytes");
STATIC_ASSERT(sizeof(u128) == 16,  "u128 must be 16 bytes");

STATIC_ASSERT(sizeof(s8)   == 1,  "s8   must be 1  byte");
STATIC_ASSERT(sizeof(s16)  == 2,  "s16  must be 2  bytes");
STATIC_ASSERT(sizeof(s32)  == 4,  "s32  must be 4  bytes");
STATIC_ASSERT(sizeof(s64)  == 8,  "s64  must be 8  bytes");
STATIC_ASSERT(sizeof(s128) == 16, "s128 must be 16 bytes");

typedef u8 bool;

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

	/* #include "./str_error.h" */
	/* #define PERROR_LOG(format, ...) printf(COLOR_STR("WARNING:" __FILE__ ":%u: ", WARNING_COLOR) format ", because: " COLOR_STR("'%s'", BRIGHT_YELLOW) ".\n", __LINE__, ##__VA_ARGS__, str_error()) */
	
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
#define CAST_PTR(ptr, type) ((type*) (ptr))
#define MAX(a, b) ((a) > (b) ? (a) : (b)) 
#define MIN(a, b) ((a) < (b) ? (a) : (b)) 
#define ARR_SIZE(arr) (sizeof(arr) / sizeof(*(arr)))

#ifdef _CHONKY_NUMS_UTILS_IMPLEMENTATION_

#define CHONKY_ASSERT(condition) chonky_assert(condition, #condition, __FILE__, __LINE__, __func__)
UNUSED_FUNCTION static void chonky_assert(bool condition, const char* condition_str, const char* file, const int line, const char* func) {
	if (condition) return;
	printf(COLOR_STR("ASSERT::%s:%u: ", ERROR_COLOR) "Failed to assert condition " COLOR_STR("'%s'", BLUE) " in function " COLOR_STR("'%s'", PURPLE) "\n", file, line, condition_str, func);
	abort();
	return;
}

UNUSED_FUNCTION static u64 str_len(const char* str) {
	if (str == NULL) return 0;
	u64 i = 0;
	while (*str++) ++i;
	return i;
}

UNUSED_FUNCTION static void mem_move(void* dest, const void* src, size_t size) {
    if (dest == NULL || src == NULL || size == 0) return;
    
	unsigned char* temp = (unsigned char*) calloc(size, sizeof(unsigned char));
	for (size_t i = 0; i < size; ++i) *CAST_PTR(temp + i, u8) = *CAST_PTR(CAST_PTR(src, u8) + i, u8); 
    for (size_t i = 0; i < size; ++i) *CAST_PTR(CAST_PTR(dest, u8) + i, u8) = *CAST_PTR(temp + i, u8);
    
	SAFE_FREE(temp);
    
    return;
}

UNUSED_FUNCTION static void* mem_cpy(void* dest, const void* src, size_t size) {
	if (dest == NULL || src == NULL) return NULL;
	for (size_t i = 0; i < size; ++i) CAST_PTR(dest, u8)[i] = CAST_PTR(src, u8)[i];
	return dest;
}

UNUSED_FUNCTION static int mem_cmp(const void* a, const void* b, size_t size) {
	if (a == NULL || b == NULL) return -2;
	for (size_t i = 0; i < size; ++i) {
		if (CAST_PTR(a, u8)[i] != CAST_PTR(b, u8)[i]) return -1;
	}
	return 0;
}

#define mem_set(ptr, value, size)    mem_set_var(ptr, value, size, sizeof(u8))
#define mem_set_32(ptr, value, size) mem_set_var(ptr, value, size, sizeof(u32))
#define mem_set_64(ptr, value, size) mem_set_var(ptr, value, size, sizeof(u64))
UNUSED_FUNCTION static void mem_set_var(void* ptr, int value, size_t size, size_t val_size) {
	if (ptr == NULL) return;
	for (size_t i = 0; i < size; ++i) CAST_PTR(ptr, u8)[i] = CAST_PTR(&value, u8)[i % val_size]; 
	return;
}

#endif // _CHONKY_NUMS_UTILS_IMPLEMENTATION_

// TODO: Use the data_64 entry instead of the one u8 one when casting
typedef struct BigNum {
	union {
		u8* data;
		u64* data_64;
	};
	u64 size;
	u8 sign;
} BigNum;

#define PRINT_CHONKY_NUM(num) print_chonky_num(#num, num)
void print_chonky_num(char* name, BigNum num) {
	printf("%s[%llu]: %s", name, num.size, num.sign ? "-" : "");
	for (s64 i = num.size - 1; i >= 0; --i) {
		printf("%02X", num.data[i]);
	}
	printf("\n");
	return;
}

static inline u64 align_64(u64 val) {
	return val + (val % 8 ? (8 - (val % 8)) : 0);
}

BigNum alloc_chonky_num(const u8 data[], const u64 size, bool sign) {
	BigNum num = { .sign = sign, .size = align_64(size) };
	
	num.data = calloc(num.size, sizeof(u8));
	if (num.data == NULL) {
		WARNING_LOG("Failed to allocate data buffer.");
		return (BigNum) {0};
	}
	
	mem_cpy(num.data, data, size);
	
	return num;
}

static BigNum dup_chonky_num(BigNum num) {
	BigNum duped = num;

	duped.data = calloc(duped.size, sizeof(u8));
	if (duped.data == NULL) {
		WARNING_LOG("Failed to allocate data buffer.");
		return (BigNum) {0};
	}

	mem_cpy(duped.data, num.data, num.size);
	
	return duped;
}

static u64 chonky_real_size(BigNum num) {
	u64 size = num.size;
	for (s64 i = num.size - 1; i >= 0; --i) {
		if (num.data[i]) break;
		size--;
	}
	return size;
}

static u64 chonky_real_size_64(BigNum num) {
	u64 size = num.size / 8;
	for (s64 i = (num.size / 8) - 1; i >= 0; --i) {
		if (num.data_64[i]) break;
		size--;
	}
	return size;
}

static int chonky_resize(BigNum* num, u64 new_size) {
	new_size = align_64(new_size);

	num -> data = realloc(num -> data, new_size * sizeof(u8));
	if (num -> data == NULL) {
		WARNING_LOG("Failed to allocate data buffer.");
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
    }
    
	va_end(args);
	
	return;
}

bool chonky_is_gt(BigNum a, BigNum b) {
	if (a.data == NULL || b.data == NULL) {
		WARNING_LOG("Parameters must be not null.");
		return FALSE;
	}

	u64 a_size = chonky_real_size(a);
	u64 b_size = chonky_real_size(b);

	if (a_size > b_size) return TRUE;
	else if (a_size < b_size) return FALSE;

	for (s64 i = a_size - 1; i >= 0; --i) {
		if (a.data[i] > b.data[i]) return TRUE;
		else if (b.data[i] > a.data[i]) return FALSE;
	}
	
	return FALSE;
}

bool is_chonky_zero(BigNum num) {
	if (num.data == NULL) {
		WARNING_LOG("Parameters must be not null.");
		return FALSE;
	}

	for (u64 i = 0; i < num.size; ++i) {
		if (num.data_64[i]) return FALSE;
	}
	
	return TRUE;
}

static BigNum __chonky_add(BigNum res, BigNum a, BigNum b) {
	const u64 size_64 = res.size / 8; 
	
	u64 carry = 0;
	for (u64 i = 0; i < size_64; ++i) {
		u64* acc = (u64*) (res.data + i * 8);
		const u64 a_val = (i < a.size) ? ((u64*) a.data)[i] : 0;
		const u64 b_val = (i < b.size) ? ((u64*) b.data)[i] : 0;
		carry = _addcarry_u64(carry, a_val, b_val, acc);
	}

	return res;
}

static BigNum __chonky_sub(BigNum res, BigNum a, BigNum b) {
	const u64 size_64 = res.size / 8; 
	
	u64 carry = 0;
	for (u64 i = 0; i < size_64; ++i) {
		u64* acc = (u64*) (res.data + i * 8);
		const u64 a_val = (i < a.size) ? ((u64*) a.data)[i] : 0;
		const u64 b_val = (i < b.size) ? ((u64*) b.data)[i] : 0;
		carry = _subborrow_u64(carry, a_val, b_val, acc);
	}
	
	if (res.sign) {
		for (u64 i = 0; i < size_64; ++i) {
			u64* acc = (u64*) (res.data + i * 8);
			*acc = ~*acc + 1;	
		}
	}

	return res;
}

static BigNum __chonky_mul_s(BigNum res, BigNum a, BigNum b) {
	for (u64 i = 0; i < a.size / 8; ++i) {
		for (u64 j = 0; j < b.size / 8; ++j) {
			*((u128*) res.data + (i + j) * 8) += (u128) ((u64*) a.data)[i] * ((u64*) b.data)[j];
		}
	}
	return res;
}

static void __chonky_divstep(u64 size_diff, BigNum a, BigNum b, u64 c) {
	const u64 size_64 = b.size / 8; 
	
	u64 i = 0;
	u64 carry = 0;
	for (i = 0; i < size_64; ++i) {
		u128 val = (u128) b.data_64[i] * c;
		carry = _subborrow_u64(carry, a.data_64[i + size_diff], (u64) val, a.data_64 + i + size_diff);
		carry += (u64) (val >> 64);
	}

	CHONKY_ASSERT(!carry);
	// TODO: This should probably not be necessary
	// if (carry) _subborrow_u64(carry, a.data_64[i + size_diff], 0, a.data_64 + i + size_diff);
	
	return;
}

static BigNum __chonky_div(BigNum res, BigNum a, BigNum b) {
	CHONKY_ASSERT(a.size >= b.size);
	
	BigNum a_c = dup_chonky_num(a);
	BigNum b_c = dup_chonky_num(b);
	
	a_c.sign = 0;
	b_c.sign = 0;

	const u64 low_limit  = (b.size / 8) - 1;

	u64 i = chonky_real_size_64(a_c) - 1;
	while (!chonky_is_gt(b_c, a_c)) {
		// Perform the division with the upper components
		const u8 additional = (low_limit > 0) ? (b_c.data_64[low_limit - 1] && (b_c.data_64[low_limit - 1] >= a_c.data_64[i - 1])) : 0;
		u64 q_hat = a_c.data_64[i] / (b_c.data_64[low_limit] + additional);
		__chonky_divstep(i - low_limit, a_c, b_c, q_hat);
		res.data_64[i - low_limit] += q_hat;
		i = chonky_real_size_64(a_c) - 1;
	}
	
	DEALLOC_CHONKY_NUMS(&a_c, &b_c);

	return res;
}

BigNum chonky_add(BigNum a, BigNum b) {
	if (a.data == NULL || b.data == NULL) {
		WARNING_LOG("Parameters must be not null.");
		return (BigNum) {0};
	}

	BigNum res = { .size = align_64(MAX(a.size, b.size) + 1) };

	res.data = calloc(res.size, sizeof(u8));
	if (res.data == NULL) {
		WARNING_LOG("Failed to allocate data buffer.");
		return (BigNum) {0};
	}

	res.sign = (a.sign && b.sign)             || 
		       (chonky_is_gt(a, b) && a.sign) || 
			   (chonky_is_gt(b, a) && b.sign);
	
	if ((a.sign && !b.sign) || (!a.sign && b.sign)) {
		const BigNum minuend = a.sign ? b : a;
		const BigNum subtraend = a.sign ? a : b;
		res = __chonky_sub(res, minuend, subtraend);
	} else res = __chonky_add(res, a, b);

	return res;
}

BigNum chonky_sub(BigNum a, BigNum b) {
	b.sign = !b.sign;
	BigNum res = chonky_add(a, b);
	return res;
}

BigNum chonky_mul(BigNum a, BigNum b) {
	if (a.data == NULL || b.data == NULL) {
		WARNING_LOG("Parameters must be not null.");
		return (BigNum) {0};
	}

	BigNum res = { .size = align_64(a.size + b.size) };

	res.data = calloc(res.size, sizeof(u8));
	if (res.data == NULL) {
		WARNING_LOG("Failed to allocate data buffer.");
		return (BigNum) {0};
	}

	res.sign = a.sign | b.sign;
	
	res = __chonky_mul_s(res, a, b);

	return res;
}

BigNum chonky_div(BigNum a, BigNum b) {
	if (a.data == NULL || b.data == NULL) {
		WARNING_LOG("Parameters must be not null.");
		return (BigNum) {0};
	}

	BigNum res = { .size = align_64(MAX(a.size, b.size)) };

	res.data = calloc(res.size, sizeof(u8));
	if (res.data == NULL) {
		WARNING_LOG("Failed to allocate data buffer.");
		return (BigNum) {0};
	}

	res.sign = a.sign | b.sign;
	
	res = __chonky_div(res, a, b);
	
	chonky_resize(&res, chonky_real_size(res));

	return res;
}

#endif //_CHONKY_NUMS_H_

