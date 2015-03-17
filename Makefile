debug: CCFLAGS = -g
debug: test


single_cycle: main.o instruction.o cpu.o read_file.o
	gcc -o single_cycle $^ $(CCFLAGS)
test: test.o instruction.o cpu.o read_file.o
	gcc -o test $^ $(CCFLAGS)
%.o: %.c
	gcc -c $< $(CCFLAGS)
clean:
	rm *.o
