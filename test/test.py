import os
import sys
import random
from chonky_nums import *

def int_to_bytes(x, n=64):
    return x.to_bytes(n, "little")

def bytes_to_int(b):
    return int.from_bytes(b, "little")

# TODO: Extend to use also differnet byte sizes
def test_add(chonky_nums):   
    for _ in range(10000):
        a = random.getrandbits(512)
        b = random.getrandbits(512)
        expected = a + b

        a_bytes = int_to_bytes(a)
        b_bytes = int_to_bytes(b)

        big_a = chonky_nums.alloc_chonky_num(ctypes.cast(a_bytes, ctypes.POINTER(ctypes.c_uint8)), 64, 0)
        big_b = chonky_nums.alloc_chonky_num(ctypes.cast(b_bytes, ctypes.POINTER(ctypes.c_uint8)), 64, 0)

        res = chonky_nums.chonky_add(big_a, big_b)
        if not res:
            chonky_nums.dealloc_chonky_num(big_a)
            chonky_nums.dealloc_chonky_num(big_b)
            continue
        
        result = bytes_to_int(ctypes.string_at(res.contents.data, res.contents.size))
        
        # Fix Sign
        result *= 1 - 2 * res.contents.sign

        chonky_nums.dealloc_chonky_num(big_a)
        chonky_nums.dealloc_chonky_num(big_b)
        chonky_nums.dealloc_chonky_num(res)

        assert result == expected, f"Mismatch:\nA={a:x}\nB={b:x}\nRes={result:x}\nExp={expected:x}"
    return

def test_sub(chonky_nums):   
    for i in range(10000):
        # TODO: Test for fail
        # Res=ffffffffffffffffffd191552ff3266787fa8d359f8fa079e5d5737bd5c393eed4b12368ff0e3fbe5dc29ee1ffb3c480473e63540025250856979f9fe42804674966938f4be7b727
        # Exp=-2e6eaad00cd998780572ca60705f861a2a8c842a3c6c112b4edc9700f1c041a23d611e004c3b7fb8c19cabffdadaf7a96860601bd7fb98b6996c70b41848d9
        
        if i == 0:
            a = int("742e5965d2886794bd581540d9e441a2844b75359639b520c5a2fae48dff2637d437676b122407421a03714a974f7688625776c65cc81b97003b72042f56c6", 16)
            b = int("a29d0435df62000cc2cadfa14a43c7bcaed7f95fd2a5c64c147f91e57fbf67da1198856b5e5f86fadba01d4a722a6e31cab7d6e234c3b44d99a7e2b8479f9f", 16)
        else: 
            a = random.getrandbits(512)
            b = random.getrandbits(512)
        
        expected = a - b

        a_bytes = int_to_bytes(a)
        b_bytes = int_to_bytes(b)

        big_a = chonky_nums.alloc_chonky_num(ctypes.cast(a_bytes, ctypes.POINTER(ctypes.c_uint8)), 64, 0)
        big_b = chonky_nums.alloc_chonky_num(ctypes.cast(b_bytes, ctypes.POINTER(ctypes.c_uint8)), 64, 0)

        res = chonky_nums.chonky_sub(big_a, big_b)
        if not res:
            chonky_nums.dealloc_chonky_num(big_a)
            chonky_nums.dealloc_chonky_num(big_b)
            continue
        
        result = bytes_to_int(ctypes.string_at(res.contents.data, res.contents.size))
        
        # Fix Sign
        result *= 1 - 2 * res.contents.sign

        chonky_nums.dealloc_chonky_num(big_a)
        chonky_nums.dealloc_chonky_num(big_b)
        chonky_nums.dealloc_chonky_num(res)

        assert result == expected, f"Mismatch:\nA={a:x}\nB={b:x}\nRes={result:x}\nExp={expected:x}"
        assert i != 0, "check todo"
    return

def test_mul(chonky_nums):   
    for _ in range(10000):
        a = random.getrandbits(512)
        b = random.getrandbits(512)
        expected = a * b

        a_bytes = int_to_bytes(a)
        b_bytes = int_to_bytes(b)

        big_a = chonky_nums.alloc_chonky_num(ctypes.cast(a_bytes, ctypes.POINTER(ctypes.c_uint8)), 64, 0)
        big_b = chonky_nums.alloc_chonky_num(ctypes.cast(b_bytes, ctypes.POINTER(ctypes.c_uint8)), 64, 0)

        res = chonky_nums.chonky_mul(big_a, big_b)
        if not res:
            chonky_nums.dealloc_chonky_num(big_a)
            chonky_nums.dealloc_chonky_num(big_b)
            continue
        
        result = bytes_to_int(ctypes.string_at(res.contents.data, res.contents.size))

        # Fix Sign
        result *= 1 - 2 * res.contents.sign
        
        chonky_nums.dealloc_chonky_num(big_a)
        chonky_nums.dealloc_chonky_num(big_b)
        chonky_nums.dealloc_chonky_num(res)

        assert result == expected, f"Mismatch:\nA = {a:x}\nB = {b:x}\nRes = {result:x}\nExp = {expected:x}"
    return

def test_div(chonky_nums):   
    for _ in range(10000):
        a = random.getrandbits(512)
        b = random.getrandbits(512)
        expected = a // b

        # print(f"Div: {i} out of 10000\nA = {a:x}\nB = {b:x}\nExp = {expected:x}")

        a_bytes = int_to_bytes(a)
        b_bytes = int_to_bytes(b)

        big_a = chonky_nums.alloc_chonky_num(ctypes.cast(a_bytes, ctypes.POINTER(ctypes.c_uint8)), 64, 0)
        big_b = chonky_nums.alloc_chonky_num(ctypes.cast(b_bytes, ctypes.POINTER(ctypes.c_uint8)), 64, 0)

        res = chonky_nums.chonky_div(big_a, big_b)
        if not res:
            chonky_nums.dealloc_chonky_num(big_a)
            chonky_nums.dealloc_chonky_num(big_b)
            continue
        
        result = bytes_to_int(ctypes.string_at(res.contents.data, res.contents.size))

        # Fix Sign
        result *= 1 - 2 * res.contents.sign
        
        chonky_nums.dealloc_chonky_num(big_a)
        chonky_nums.dealloc_chonky_num(big_b)
        chonky_nums.dealloc_chonky_num(res)

        assert result == expected, f"Mismatch:\nA = {a:x}\nB = {b:x}\nRes = {result:x}\nExp = {expected:x}"
    return

def test_pow(chonky_nums):   
    for _ in range(10000):
        a = random.getrandbits(512)
        #b = random.getrandbits(8)
        b = 3
        expected = a ** b
        
        n1 = a
        print(f"n1 = {n1:X}")
        n2 = a * a
        print(f"n2 = {n2:X}")
        n3 = n1 * n2
        print(f"n3 = {n3:X}")

        a_bytes = int_to_bytes(a)
        b_bytes = int_to_bytes(b, 2)

        big_a = chonky_nums.alloc_chonky_num(ctypes.cast(a_bytes, ctypes.POINTER(ctypes.c_uint8)), 64, 0)
        big_b = chonky_nums.alloc_chonky_num(ctypes.cast(b_bytes, ctypes.POINTER(ctypes.c_uint8)), 1, 0)

        res = chonky_nums.chonky_pow(big_a, big_b)
        if not res:
            chonky_nums.dealloc_chonky_num(big_a)
            chonky_nums.dealloc_chonky_num(big_b)
            continue
    
        result = bytes_to_int(ctypes.string_at(res.contents.data, res.contents.size))

        # Fix Sign
        result *= 1 - 2 * res.contents.sign
        
        chonky_nums.dealloc_chonky_num(big_a)
        chonky_nums.dealloc_chonky_num(big_b)
        chonky_nums.dealloc_chonky_num(res)

        assert result == expected, f"Mismatch:\nA = {a:x}\nB = {b:x}\nRes = {result:x}\nExp = {expected:x}"
        assert i != 0
    return

def test_mod(chonky_nums):   
    for _ in range(10000):
        a = random.getrandbits(512)
        b = random.getrandbits(512)
        expected = a % b

        a_bytes = int_to_bytes(a)
        b_bytes = int_to_bytes(b)

        big_a = chonky_nums.alloc_chonky_num(ctypes.cast(a_bytes, ctypes.POINTER(ctypes.c_uint8)), 64, 0)
        big_b = chonky_nums.alloc_chonky_num(ctypes.cast(b_bytes, ctypes.POINTER(ctypes.c_uint8)), 64, 0)

        res = chonky_nums.chonky_mod(big_a, big_b)
        if not res:
            chonky_nums.dealloc_chonky_num(big_a)
            chonky_nums.dealloc_chonky_num(big_b)
            continue
        
        result = bytes_to_int(ctypes.string_at(res.contents.data, res.contents.size))

        # Fix Sign
        result *= 1 - 2 * res.contents.sign
        
        chonky_nums.dealloc_chonky_num(big_a)
        chonky_nums.dealloc_chonky_num(big_b)
        chonky_nums.dealloc_chonky_num(res)

        assert result == expected, f"Mismatch:\nA = {a:x}\nB = {b:x}\nRes = {result:x}\nExp = {expected:x}"
    return

def test_mod_mersenne(chonky_nums):   
    for _ in range(10000):
        a = random.getrandbits(512)
        b = 2 ** 255 - 19
        expected = a % b

        a_bytes = int_to_bytes(a)
        b_bytes = int_to_bytes(b)

        big_a = chonky_nums.alloc_chonky_num(ctypes.cast(a_bytes, ctypes.POINTER(ctypes.c_uint8)), 64, 0)
        big_b = chonky_nums.alloc_chonky_num(ctypes.cast(b_bytes, ctypes.POINTER(ctypes.c_uint8)), 64, 0)

        res = chonky_nums.chonky_mod_mersenne(big_a, big_b)
        if not res:
            chonky_nums.dealloc_chonky_num(big_a)
            chonky_nums.dealloc_chonky_num(big_b)
            continue
        
        result = bytes_to_int(ctypes.string_at(res.contents.data, res.contents.size))

        # Fix Sign
        result *= 1 - 2 * res.contents.sign
        
        chonky_nums.dealloc_chonky_num(big_a)
        chonky_nums.dealloc_chonky_num(big_b)
        chonky_nums.dealloc_chonky_num(res)

        assert result == expected, f"Mismatch:\nA = {a:x}\nB = {b:x}\nRes = {result:x}\nExp = {expected:x}"
    return

if __name__ == "__main__":
    chonky_nums = ChonkyNums()

    # print("Testing chonky_add...")
    # test_add(chonky_nums)
    
    # print("Testing chonky_sub...")
    # test_sub(chonky_nums)
    
    # print("Testing chonky_mul...")
    # test_mul(chonky_nums)
    
    # print("Testing chonky_div...")
    # test_div(chonky_nums)

    print("Testing chonky_pow...")
    test_pow(chonky_nums)

    print("Testing chonky_mod...")
    test_mod(chonky_nums)

    print("Testing chonky_mod_mersenne...")
    test_mod_mersenne(chonky_nums)

    print("Test passed")

