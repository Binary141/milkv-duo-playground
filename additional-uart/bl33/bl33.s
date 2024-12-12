# note: this code comes from the sophgo forms as an example of creating your own bl33
# https://forum.sophgo.com/t/use-opensbi-to-boot-your-own-operating-system/340

#define UART0_THR 0x04140000
#define UART0_LSR 0x04140014

# .equ UART0_LSR, 0x04140014

.section .text
.global _start
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
	/* Information end */

real_start:
    j real_start
