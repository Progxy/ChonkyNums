FLAGS = -std=gnu11 -Wall -Wextra -pedantic -ggdb
FLAGS += -Wno-gnu-zero-variadic-macro-arguments -Wno-empty-body
DEFINITIONS = -D_DEBUG 

example: example.c ../chonky_nums.h
	gcc $(FLAGS) $(DEFINITIONS) $< -o $@

temp: temp.c ../chonky_nums.h
	gcc $(FLAGS) $(DEFINITIONS) $< -o $@
