debug: CCFLAGS = -g
debug: test


single_cycle: main.o instruction.o cpu.o readfile.o
	gcc -o single_cycle $^ $(CCFLAGS)
test: test.o instruction.o cpu.o readfile.o i_type.o j_type.o r_type.o
	gcc -o test $^ $(CCFLAGS)
%.o: %.c
	gcc -c $< $(CCFLAGS)
clean:
	rm *.o
