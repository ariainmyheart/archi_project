CCFLAGS = -Wall -g
main_src = $(wildcard *.c)
main_obj= $(patsubst %.c, %.o, $(main_src))
test_src = $(wildcard test/*.c)
test_obj = $(patsubst %.c, %.o, $(test_src))

all: $(main_obj)
	gcc -o pipeline $^ $(CCFLAGS)
$(main_obj): %.o: %.c
	gcc -o $@ -c $< $(CCFLAGS)
clean:
	rm *.o
