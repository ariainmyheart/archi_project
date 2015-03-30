CCFLAGS = -Wall
objects = instruction.o cpu.o readfile.o i_type.o j_type.o r_type.o error.o
test_objects = test/test_cpu.o test/test_readfile.o

single_cycle: main.o $(objects)
	gcc -o single_cycle $^ $(CCFLAGS)
test: CCFLAGS += -g
test: test/test.o $(objects) $(test_objects)
	gcc $^ $(CCFLAGS) -o test/$@
%.o: %.c
	gcc -c $< $(CCFLAGS)
test/%.o: test/%.c
	gcc -c $< $(CCFLAGS) -o $@
clean:
	rm *.o
	rm test/*.o

