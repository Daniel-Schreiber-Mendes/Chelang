

c_src_files := $(shell find . -name '*.c')
c_obj_files := $(c_src_files:.c=.o)


make: $(c_obj_files)
	gcc -o ccc $(^F) -lchecl
	./ccc

%.o : %.c
	gcc -c -g '$^' -Wall -Wextra -Wno-unused-parameter -Wno-unused-function -Wno-sign-compare -lcheio -Iinclude