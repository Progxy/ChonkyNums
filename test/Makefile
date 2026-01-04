FLAGS = -std=gnu11 -Wall -Wextra -pedantic -ggdb
FLAGS += -Wno-gnu-zero-variadic-macro-arguments -Wno-empty-body
DEFINITIONS = -D_DEBUG 

all: chonky_nums_py.so chonky_nums.py
	python3 test.py

chonky_nums.py: 
	python3 autogen.py ../chonky_nums.h $@

chonky_nums_py.so: chonky_nums.c
	gcc $(FLAGS) -fPIC -shared $(DEFINITIONS) $< -o $@ 

