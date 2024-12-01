#define UART0_THR 0x04140000
#define UART0_LSR 0x04140014

.equ UART0_THR, 0x04140000
.equ UART0_LSR, 0x04140014

.section .text
.global _start
.global _entry

_entry:
    j _start
    ret

_start:
	/* BL33 information */
	j real_start
	.balign 4
	.word 0x33334c42  /* b'BL33' */
	.word 0xdeadbeea  /* CKSUM */
	.word 0xdeadbeeb  /* SIZE */
	.quad 0x80200000  /* RUNADDR */
	.word 0xdeadbeec
	.balign 4
	j real_start
	.balign 4
	/* Information end */

real_start:
	la s0, str
1:
	lbu a0, (s0)
	beqz a0, exit
	jal ra, uart_send
	addi s0, s0, 1
	j 1b

exit:
	j exit

uart_send:
	/* Wait for tx idle */
	li t0, UART0_LSR
	lw t1, (t0)
	andi t1, t1, 0x20
	beqz t1, uart_send
	/* Send a char */
	li t0, UART0_THR
	sw a0, (t0)
	ret

.section .rodata
str: 
	.asciz "\nHello Milkv-duo!"
