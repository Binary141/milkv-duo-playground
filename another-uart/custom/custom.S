.section .text
.global _start
.global _entry

    .equ GPIO_2, 0x03022000
    .equ OFFSET, 0x004

_entry:
    j _start
    ret

_start:
	j real_start

real_start:
    li t0, GPIO_2
    li t1, 1<<24
    sw t1, OFFSET(t0) # make the pin an output

    # below is code used from xv6-riscv 

    # set up a stack for C.
    # stack0 is declared in start.c,
    # with a 4096-byte stack per CPU.
    # sp = stack0 + (hartid * 4096)
    la sp, stack0
    li a0, 1024*4
    csrr a1, mhartid
    addi a1, a1, 1
    mul a0, a0, a1
    add sp, sp, a0

    call main

spin:
    j spin
