import os
import sys
import random
from chonky_nums import *

def int_to_bytes(x, n=64):  # 512 bits
    return x.to_bytes(n, "little")

def bytes_to_int(b):
    return int.from_bytes(b, "little")

def bytes_to_uint8_ptr(b: bytes):
    arr = (ctypes.c_uint8 * len(b))(*b)  # Create C array and copy bytes into it
    return arr, ctypes.cast(arr, ctypes.POINTER(ctypes.c_uint8))

# TODO: Extend to use also differnet byte sizes
def test_add(chonky_nums):   
    for _ in range(10000):
        a = random.getrandbits(512)
        b = random.getrandbits(512)
        expected = a + b

        a_bytes = int_to_bytes(a)
        b_bytes = int_to_bytes(b)

        print(a_bytes.hex().upper())
        big_a = chonky_nums.alloc_chonky_num(ctypes.cast(a_bytes, ctypes.POINTER(ctypes.c_uint8)), 64, 0)
        
        print(f"\n{b_bytes.hex().upper()}")
        big_b = chonky_nums.alloc_chonky_num(ctypes.cast(b_bytes, ctypes.POINTER(ctypes.c_uint8)), 64, 0)

        res = chonky_nums.chonky_add(big_a, big_b)
        result = bytes_to_int(ctypes.string_at(res.contents.data, res.contents.size))
        print(f"\n*res: {ctypes.string_at(res.contents.data, res.contents.size).hex().upper()}")

        assert result == expected, f"Mismatch:\nA={a:x}\nB={b:x}\nRes={result:x}\nExp={expected:x}"
    return

if __name__ == "__main__":
    chonky_nums = ChonkyNums()

    test_add(chonky_nums)

    print("Test passed")
