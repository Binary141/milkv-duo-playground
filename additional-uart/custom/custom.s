# .equ UART0_THR, 0x04140000
# .equ UART0_LSR, 0x04140014

# this are the addresses for uart 4
.equ UART4_BASE, 0x04160000

# uart offsets
.equ LCR_OFFSET, 0x00c
.equ DLL_OFFSET, 0x000
.equ RBR_OFFSET, 0x000
.equ IER_DLH_OFFSET, 0x004
.equ FCR_IIR_OFFSET, 0x008
.equ MCR_OFFSET, 0x010
.equ THR_OFFSET, 0x000
.equ LSR_OFFSET, 0x014
.equ FAR_OFFSET, 0x070
.equ SRR_OFFSET, 0x088

.equ IN_FREQ, 25000000

# there is no guarantee that this is the final value due to possible rounding errors in the calculation
.equ BAUD, 115200

.equ GPIO_2, 0x03022000
.equ OFFSET, 0x004

# some large value
.equ THRESHOLD, 0xF000000

.equ CLK_DIV_CRG, 0x03002000
.equ CLK_EN_1_OFFSET, 0x004

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

    # init the uart by hand
    # initialization steps
    # 1. Write 1 to LCR [7]. Enable and configure division latch access
    # 2. Write the corresponding configuration value to RBR_THR_DLL IER_DLH register, configure the baud rate of transmission
    # 3. Write 0 to LCR [7]
    # 4. Configure LCR and set corresponding UART working mode
    # 5. Configure FCR and set corresponding transmit and receive FIFO threshold.
    # 6. If interrupt mode is used, IER should be set to enable corresponding interrupt signal.
    
    # enable the clock for uart 4
    li t1, CLK_DIV_CRG
    lw t0, CLK_EN_1_OFFSET(t1)
    li t2, 1<<23
    or t3, t0, t2
    sw t3, CLK_EN_1_OFFSET(t1)

    li t0, THRESHOLD         # small delay to wait for clock to stabilize
    wait_clock:
        addi t0, t0, -1
        bnez t0, wait_clock

    # configure the MCR
    # li t0, 1<<7
    # li t1, UART4_BASE
    # sw t0, LCR_OFFSET(t1)

    # li t0, 1<<5
    # li t1, UART4_BASE
    # sw t0, MCR_OFFSET(t1)

    # Step 1
    li t0, 1<<7
    li t1, UART4_BASE
    sw t0, LCR_OFFSET(t1)

    # Step 2a
    li t0, 0x0e
    li t1, UART4_BASE
    sw t0, DLL_OFFSET(t1)

    # Step 2b
    li t0, 0x00
    li t1, UART4_BASE
    sw t0, IER_DLH_OFFSET(t1)

    # Step 3
    li t0, 0<<7
    li t1, UART4_BASE
    sw t0, LCR_OFFSET(t1)

    # Step 4
    li t0, 0x03 # 8 bits with one stop bit
    li t1, UART4_BASE
    sw t0, LCR_OFFSET(t1)

    # Step 5
    li t0, 0x06  # Clear transmit & receive FIFOs
    li t1, UART4_BASE
    sw t0, FCR_IIR_OFFSET(t1)

    li t0, 0x01  # Enable FIFO (bit 0)
    sw t0, FCR_IIR_OFFSET(t1)

    li t0, 0x00
    li t1, UART4_BASE
    sw t0, MCR_OFFSET(t1)

    # read the line status
    li t1, UART4_BASE
    lbu t0, LSR_OFFSET(t1)

    li t1, UART4_BASE
    lbu t0, FCR_IIR_OFFSET(t1)

    # read the receive buffer
    li t1, UART4_BASE
    lbu t0, RBR_OFFSET(t1)

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
	lbu a0, (s0)
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
	lbu a0, (s0)
	jal ra, uart_send
    j 5b
4:
    sw t2, (t0) # turn the pin off
    li t5, 0
    j 5b

uart_send:
	/* Wait for tx idle */
	li t0, UART4_BASE
	lw t1, LSR_OFFSET(t0)
	andi t1, t1, 0x20
	beqz t1, uart_send
	/* Send a char */
	li t0, UART4_BASE
	sw a0, THR_OFFSET(t0)
	ret

.section .rodata
str: 
	.asciz "\nTry to blink!"
