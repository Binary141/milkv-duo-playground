all: run

compile:
	gcc -c main.c

riscv64:
	./riscv64-musl/bin/riscv64-buildroot-linux-musl-gcc --static main.c

link: compile
	gcc -o main main.o

run: link
	./main
