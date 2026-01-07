FLAGS = -std=gnu11 -Wall -Wextra -pedantic -ggdb
FLAGS += -Wno-gnu-zero-variadic-macro-arguments -Wno-empty-body
FLAGS += -fsanitize=undefined -fsanitize=address -O2
DEFINITIONS = -D_DEBUG 

example: example.c ../chonky_nums.h
	gcc $(FLAGS) $(DEFINITIONS) $< -o $@

temp: temp.c ../chonky_nums.h
	gcc $(FLAGS) $(DEFINITIONS) $< -o $@
