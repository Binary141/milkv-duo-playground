all: build

build:
	riscv64-unknown-linux-musl-gcc -T custom.ld -nostdlib -fno-builtin -march=rv64gc -mabi=lp64f -g -Wall -o custom.elf custom.s
	/opt/riscv/riscv64-unknown-linux-musl/bin/objcopy -O binary custom.elf custom.bin

clean:
	rm -rf ./*.o
	rm -rf ./*.elf
	rm -rf ./*.bin
