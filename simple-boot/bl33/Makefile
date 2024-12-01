all: bl33

bl33:
	riscv64-unknown-linux-musl-gcc -nostdlib -fno-builtin -march=rv64gc -mabi=lp64f -g -Wall -Ttext=0x80200000 -o bl33.elf bl33.s
	/opt/riscv/riscv64-unknown-linux-musl/bin/objcopy -O binary bl33.elf bl33.bin

clean:
	rm -rf bl33.bin
	rm -rf bl33.elf
