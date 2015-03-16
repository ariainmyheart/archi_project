single_cycle: main.o cpu.o read_file.o
	gcc -o single_cycle $^
test: test.o cpu.o read_file.o
	gcc -o test $^
%.o: %.c
	gcc -c $<
clean:
	rm *.o
