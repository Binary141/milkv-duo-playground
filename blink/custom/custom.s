#define UART0_THR 0x04140000
#define UART0_LSR 0x04140014

.equ UART0_THR, 0x04140000
.equ UART0_LSR, 0x04140014
.equ GPIO_2, 0x03022000
.equ OFFSET, 0x004
.equ THRESHOLD, 0xF000000

.section .text
.global _start
.global _entry

_entry:
    j _start
    ret

_start:
	j real_start

real_start:
	la s0, str
1:
	lbu a0, (s0)
	beqz a0, blink
	jal ra, uart_send
	addi s0, s0, 1
	j 1b

blink:
    li t0, GPIO_2
    li t1, 1<<24
    li t2, 0<<24
    li t3, THRESHOLD

    sw t1, OFFSET(t0) # make the pin an output

    j loop

loop:
    li t0, GPIO_2 # gpio base address
    li t1, 1<<24 # pin select high
    li t2, 0<<24 # pin select low
    li t3, THRESHOLD # upper bound
    li t4, 0 # counter
    li t5, 1 # toggle val

    sw t1, (t0) # turn the pin on

    /*
    The below labels roughly equate to this logic

    while(1) {
        if (i == THRESHOLD) {
            if (toggle == 1) {
                led_on;
                toggle = 1;
            } else {
                led_off;
                toggle = 0;
            }
            i = 0
        }
        i++;
    }
    */
1:
    addi t4, t4, 1
    bne t4, t3, 1b

2:
    beqz t5, 3f
    bnez t5, 4f

5:
    li t4, 0 # reset the counter
    j 1b

3:
    sw t1, (t0) # turn the pin on
    li t5, 1
    j 5b
4:
    sw t2, (t0) # turn the pin off
    li t5, 0
    j 5b

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
	.asciz "\nTry to blink!"
