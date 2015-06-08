CCFLAGS = -Wall
main_src = $(wildcard *.c)
main_obj= $(patsubst %.c, %.o, $(main_src))

all: $(main_obj)
	gcc -o CMP $^ $(CCFLAGS)
$(main_obj): %.o: %.c
	gcc -o $@ -c $< $(CCFLAGS)
clean:
	rm *.o
