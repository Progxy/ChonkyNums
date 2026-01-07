# ChonkyNums

> [!WARNING]
> Note the following branch aims to offer a constant-time (against timing based attacks), and cryptographically secure implementation.
> Furthermore, note that the following is under "active" development (take that into consideration), and only the marked functions are secure (with `CHONKY_CT`).

**ChonkyNums** is a C library for arbitrary-precision integer (big number) arithmetic, providing fast and memory-safe operations for working with "chonky" (large) numbers that exceed standard integer sizes. It includes a well-defined API for core big number manipulation, algebraic operations, type safety, and debugging/logging.

## Features

- **BigNum Structure**: Core `BigNum` type supporting arbitrarily large (arbitrary-precision) integers.
- **Safe Memory Management**: Allocation, duplication, copying, and thread-safe deallocation helpers.
- **Rich Arithmetic**:
  - Addition, subtraction, multiplication, division, and exponentiation
  - Modular reduction (with Mersenne prime optimization)
- **Utility Helpers**:
  - Real size, bit-inspection, and resizing functions
  - Macros for type casting, compound allocation, and debug assertions
- **Bitwise Manipulation**: Bit get/set, alignment utilities
- **Debugging & Logging**: Colorized formatted output, debug macros (with preprocessor flags)
- **Wide Integer Types**: Typedefs provided for 8, 16, 32, 64, and, if available, 128-bit integers

## Installation

This is a header library; simply drop `chonky_nums.h` into your project and include it:

```c

#define _CHONKY_NUMS_PRINTING_UTILS_
#define _CHONKY_NUMS_SPECIAL_TYPE_SUPPORT_
#define _CHONKY_NUMS_UTILS_IMPLEMENTATION_
#include "chonky_nums.h"

```

## Example Usage

See a more thorough example in [./example/example.c](./example/example.c).

A minimal illustration:

```c

#define _CHONKY_NUMS_PRINTING_UTILS_
#define _CHONKY_NUMS_SPECIAL_TYPE_SUPPORT_
#define _CHONKY_NUMS_UTILS_IMPLEMENTATION_
#include "chonky_nums.h"

int main(void) {
    BigNum* a = alloc_chonky_num_from_string("12345678901234567890");
    BigNum* b = alloc_chonky_num_from_string("98765432109876543210");
	BigNum* result = chonky_add(a, b);
    PRINT_CHONKY_NUM_DEC(result); // Should output: 111111111011111111100
    DEALLOC_CHONKY_NUMS(a, b, result);
    return 0;
}

```

## API Overview

- **Allocation/Memory:**
  - `alloc_chonky_num()`, `alloc_chonky_num_from_string()`, `dup_chonky_num()`
  - `dealloc_chonky_num()`, `dealloc_chonky_nums()`
- **Arithmetic:**
  - `chonky_add()`, `chonky_sub()`, `chonky_mul()`, `chonky_div()`
  - `chonky_pow()`, `chonky_mod()`, `chonky_mod_mersenne()`
- **Helpers:**
  - Macros for alignment, debug assertions, safe type-casting
  - Bit manipulation utilities

See [chonky_nums.h](./chonky_nums.h) for full API details.

## Contributing

PRs and issues are welcome! Please add tests for new features.

## License

[GNU GPL v3](./LICENSE)

---

**ChonkyNums** – Because sometimes your numbers need to be chonky.

