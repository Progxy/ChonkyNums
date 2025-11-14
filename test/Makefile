FLAGS = -std=gnu11 -Wall -Wextra -pedantic -ggdb -Wno-discarded-qualifiers
DEFINITIONS = -D_DEBUG 

all: chonky_nums_py.so chonky_nums.py
	python test.py

chonky_nums.py: 
	python autogen.py ../chonky_nums.h $@

chonky_nums_py.so: chonky_nums.c
	gcc $(FLAGS) -fPIC -shared $(DEFINITIONS) $< -o $@ 

