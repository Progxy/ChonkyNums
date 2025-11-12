FLAGS = -std=gnu11 -Wall -Wextra -pedantic -ggdb
DEFINITIONS = -D_DEBUG -D_CHONKY_NUMS_PRINTING_UTILS_ -D_CHONKY_NUMS_SPECIAL_TYPE_SUPPORT_ -D_CHONKY_NUMS_UTILS_IMPLEMENTATION_

all: test.so chonky.py
	python test.py

chonky.py: 
	python autogen.py ../chonky_nums.h $@

test.so: ../chonky_nums.h
	gcc -fPIC -shared $(FLAGS) $(DEFINITIONS) $< -o $@ 

