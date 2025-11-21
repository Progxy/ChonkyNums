import os
import sys
import random
from chonky_nums import *

import time

def timed(func):
    def wrapper(*args, **kwargs):
        start = time.perf_counter()
        result = func(*args, **kwargs)
        end = time.perf_counter()
        print(f"{func.__name__} took {end - start:.6f} seconds")
        return result
    return wrapper

def int_to_bytes(x, n=64):
    return x.to_bytes(n, "little")

def bytes_to_int(b):
    return int.from_bytes(b, "little")

@timed
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

@timed
def test_sub(chonky_nums):   
    for i in range(10000):
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
    return

@timed
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

@timed
def test_div(chonky_nums):   
    for i in range(10000):
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

@timed
def test_pow(chonky_nums):   
    for i in range(1000):
        print(f"Testing {i + 1} out of 1000", end='\r')
        a = random.getrandbits(512)
        b = random.getrandbits(8)
        expected = a ** b
        
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
    
    print("                                                         ") 
    
    return

@timed
def test_mod(chonky_nums):   
    for i in range(10000):
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

@timed
def test_mod_mersenne(chonky_nums):   
    for i in range(10000):
        a = random.getrandbits(512)
        a = int("e4bb3fe5e6204348e0959c1155d0bbadb13b19eaf3b91047e6509a41a6891fb109fa6bc9974500fc3e17982ef52b23f411ecfba1e83a1e069405b077c87e0a30", 16)
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

@timed
def test_pow_mod(chonky_nums):   
    for i in range(10000):
        print(f"Testing {i + 1} out of 10000", end='\r')
        
        a = random.getrandbits(512)
        b = random.getrandbits(8)
        c = random.getrandbits(512)
        expected = (a ** b) % c
        
        a_bytes = int_to_bytes(a)
        b_bytes = int_to_bytes(b)
        c_bytes = int_to_bytes(c)

        big_a = chonky_nums.alloc_chonky_num(ctypes.cast(a_bytes, ctypes.POINTER(ctypes.c_uint8)), 64, 0)
        big_b = chonky_nums.alloc_chonky_num(ctypes.cast(b_bytes, ctypes.POINTER(ctypes.c_uint8)), 64, 0)
        big_c = chonky_nums.alloc_chonky_num(ctypes.cast(c_bytes, ctypes.POINTER(ctypes.c_uint8)), 64, 0)

        res = chonky_nums.chonky_pow_mod(big_a, big_b, big_c)
        if not res:
            chonky_nums.dealloc_chonky_num(big_a)
            chonky_nums.dealloc_chonky_num(big_b)
            chonky_nums.dealloc_chonky_num(big_c)
            continue
        
        result = bytes_to_int(ctypes.string_at(res.contents.data, res.contents.size))

        # Fix Sign
        result *= 1 - 2 * res.contents.sign
        
        chonky_nums.dealloc_chonky_num(big_a)
        chonky_nums.dealloc_chonky_num(big_b)
        chonky_nums.dealloc_chonky_num(big_c)
        chonky_nums.dealloc_chonky_num(res)

        assert result == expected, f"Mismatch:\nA = {a:x}\nB = {b:x}\nC = {c:x}\nRes = {result:x}\nExp = {expected:x}"
    print("                                                         ") 
    
    return

@timed
def test_pow_mod_mersenne(chonky_nums):   
    c = 2 ** 255 - 19
    
    for i in range(10000):
        print(f"Testing {i + 1} out of 10000", end='\r')
        
        if i == 0:
            a = int("732F21CCB3E251385F766FA4B63A6E607CB41E6C9F72EE7CE846D9B7E3B0AE4512D98F943831D563EC726D23591CBE7D7C431B90678C8EDA29A3B4099CC10D32", 16)
            b = 0x92
        else:
            a = random.getrandbits(512)
            b = random.getrandbits(8)
        
        expected = (a ** b) % c

        # print("\n\n----------------------------------------")
        # res = 1
        # base = a % c
        # xx = base * base
        # print(f"base:{base:X}\nxx:{xx:X}")
        # for j in range(0, 8):
        #     if (b >> j) & 1:
        #         res = (res * base) % c

        #     base = (base * base) % c
        #     print(f"res={res:X}\nbase={base:X}\n")
        # print("----------------------------------------\n\n")

        a_bytes = int_to_bytes(a)
        b_bytes = int_to_bytes(b)
        c_bytes = int_to_bytes(c)

        big_a = chonky_nums.alloc_chonky_num(ctypes.cast(a_bytes, ctypes.POINTER(ctypes.c_uint8)), 64, 0)
        big_b = chonky_nums.alloc_chonky_num(ctypes.cast(b_bytes, ctypes.POINTER(ctypes.c_uint8)), 64, 0)
        big_c = chonky_nums.alloc_chonky_num(ctypes.cast(c_bytes, ctypes.POINTER(ctypes.c_uint8)), 64, 0)

        res = chonky_nums.chonky_pow_mod_mersenne(big_a, big_b, big_c)
        if not res:
            chonky_nums.dealloc_chonky_num(big_a)
            chonky_nums.dealloc_chonky_num(big_b)
            chonky_nums.dealloc_chonky_num(big_c)
            continue
        
        result = bytes_to_int(ctypes.string_at(res.contents.data, res.contents.size))

        # Fix Sign
        result *= 1 - 2 * res.contents.sign
        
        chonky_nums.dealloc_chonky_num(big_a)
        chonky_nums.dealloc_chonky_num(big_b)
        chonky_nums.dealloc_chonky_num(big_c)
        chonky_nums.dealloc_chonky_num(res)

        assert result == expected, f"Mismatch:\nA = {a:x}\nB = {b:x}\nC = {c:x}\nRes = {result:x}\nExp = {expected:x}"
        assert i != 0    
    print("                                                         ") 
    
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

    # print("Testing chonky_pow...")
    # test_pow(chonky_nums)

    # print("Testing chonky_mod...")
    # test_mod(chonky_nums)

    print("Testing chonky_mod_mersenne...")
    test_mod_mersenne(chonky_nums)

    # print("Testing chonky_pow_mod...")
    # test_pow_mod(chonky_nums)

    print("Testing chonky_pow_mod_mersenne...")
    test_pow_mod_mersenne(chonky_nums)

    print("Test passed")

