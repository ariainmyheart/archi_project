CCFLAGS = -Wall
main_src = $(wildcard *.c)
main_obj= $(patsubst %.c, %.o, $(main_src))
test_src = $(wildcard test/*.c)
test_obj = $(patsubst %.c, %.o, $(test_src))

all: $(main_obj)
	gcc -o single_cycle $^ $(CCFLAGS)
test: CCFLAGS+=-g
test: $(test_obj) $(filter-out main.o, $(main_obj))
	gcc -o test/test $^ $(CCFLAGS)
$(main_obj): %.o: %.c
	gcc -o $@ -c $< $(CCFLAGS)
$(test_obj): %.o: %.c
	gcc -o $@ -c $< $(CCFLAGS)
clean:
	rm *.o
	rm test/*.o
