all: run

riscv64:
	riscv64-unknown-linux-gnu-as -o main.o main.s
	riscv64-unknown-linux-gnu-ld main.o

clean:
	rm -f *.o
	rm -f a.out
