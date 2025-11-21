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
        # The error is in __chonky_div
        if i == 0:
            a = int("28D191827785BA07330EB310D7105E485433755E97465C5971B5F724FF449CFDE5E0FECAFC59AC35E82FDFEFBBFAC1AC6E7F6F7A11C813A83D72BB61C05D32D89366D01AB265860C5CDD45A0016CD1E4278AACED75ED7CE249F10748B1889AE7CED497967259536611ACAFD7B1A1180C686276651BECCCC441D591269889B284", 16)
            b = int("d34c70a07ebe0247167c418336f8efe6502dae4178e365782b72a0f248cb0a29b4c908d27992f597d8f5832b67d0be9ca7b97f92187896eba09a57059f5b372a", 16)
        else:
            a = random.getrandbits(512)
            b = random.getrandbits(512)
        expected = a % b

        cc = a // b
        print(f"quotient: {cc:X}")

        a_bytes = int_to_bytes(a, 128)
        b_bytes = int_to_bytes(b)

        big_a = chonky_nums.alloc_chonky_num(ctypes.cast(a_bytes, ctypes.POINTER(ctypes.c_uint8)), 128, 0)
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
        assert i != 0
    return

@timed
def test_mod_mersenne(chonky_nums):   
    for i in range(10000):
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

@timed
def test_pow_mod(chonky_nums):   
    for i in range(10000):
        print(f"Testing {i + 1} out of 10000", end='\r')
        if i == 0:
            a = int("9c03e8e0b76411a505cfa19f89780132f0822ef46f416dc3604b536b0114a32720133cafa3701f9e687767f1d0177294728c36b9e791427fffa32bf00c8f35e6", 16)
            b = int("70", 16)
            c = int("d34c70a07ebe0247167c418336f8efe6502dae4178e365782b72a0f248cb0a29b4c908d27992f597d8f5832b67d0be9ca7b97f92187896eba09a57059f5b372a", 16)
        else: 
            a = random.getrandbits(512)
            b = random.getrandbits(8)
            c = random.getrandbits(512)

        expected = (a ** b) % c
        
        res = 1
        base = a
        print("\n\n")
        print("----------------------------------------------")
        for j in range(0, 7):
            if ((b >> j) & 1):
                res = (res * base) % c

            base = (base * base) % c

            print(f"res: {res:X}\nbase: {base:X}\n")

        print("----------------------------------------------")

        t_2 = t * t
        t_2c = t_2 % c
        print(f"tst: {t_2:X}\n")
        print(f"tst: {t_2c:X}\n")

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
        assert i != 0
    print("                                                         ") 
    
    return

@timed
def test_pow_mod_mersenne(chonky_nums):   
    c = 2 ** 255 - 19
    
    for i in range(10000):
        print(f"Testing {i + 1} out of 10000", end='\r')
        
        a = random.getrandbits(512)
        b = random.getrandbits(8)
        expected = (a ** b) % c

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

    print("Testing chonky_mod...")
    test_mod(chonky_nums)

    # print("Testing chonky_mod_mersenne...")
    # test_mod_mersenne(chonky_nums)

    print("Testing chonky_pow_mod...")
    test_pow_mod(chonky_nums)

    print("Testing chonky_pow_mod_mersenne...")
    test_pow_mod_mersenne(chonky_nums)

    print("Test passed")

