import os
import sys
from chonky import *

def int_to_bytes(x, n=64):  # 512 bits
    return x.to_bytes(n, "little")

def bytes_to_int(b):
    return int.from_bytes(b, "little")

# TODO: Extend to use also differnet byte sizes
def test_add():   
    for _ in range(10000):
        a = random.getrandbits(512)
        b = random.getrandbits(512)
        expected = a + b

        a_bytes = int_to_bytes(a)
        b_bytes = int_to_bytes(b)
        big_a = 
        res_bytes = ctypes.POINTER(ctypes.c_uint8)

        chonky_nums.chonky_add(res, big_a, big_b)
        result = bytes_to_int(bytes(res.data))

        assert result == expected, f"Mismatch:\nA={a:x}\nB={b:x}\nRes={result:x}\nExp={expected:x}"
    return

if __name__ == "__main__":
    chonky_nums = chonky.ChonkyNums()

    test_add(chonky_nums)

    print("Test passed")
