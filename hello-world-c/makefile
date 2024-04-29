all: run

compile:
	gcc -c main.c

riscv64:
	riscv64-unknown-linux-gnu-gcc --static main.c

link: compile
	gcc -o main main.o

run: link
	./main

clean:
	rm a.out
