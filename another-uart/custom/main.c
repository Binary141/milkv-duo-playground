#include "uart.h"

__attribute__ ((aligned (16))) char stack0[4096 * 1];

void uartinit(void);

#define UART0 0x04140000L
#define UART4 0x04160000L

#define CLOCKGENBASEADDR 0x03002000L

#define Reg(reg) ((volatile unsigned char *)(UART4 + (reg)))

#define ClkReg(reg) ((volatile unsigned char *)(CLOCKGENBASEADDR + (reg)))
#define ReadClkReg(reg) (*(ClkReg(reg)))
#define WriteClkReg(reg, v) (*(ClkReg(reg)) = (v))

#define WriteReg(reg, v) (*(Reg(reg)) = (v))

#define UART0Reg(reg) ((volatile unsigned char *)(UART0 + (reg)))
#define WriteUART0Reg(reg, v) (*(UART0Reg(reg)) = (v))

#define LCR_BAUD_LATCH (1<<7) // special mode to set baud rate

#define RHR 0                 // receive holding register (for input bytes)
#define THR 0                 // transmit holding register (for output bytes)
#define IER 0x000                 // interrupt enable register
#define IER_RX_ENABLE (1<<0)
#define IER_TX_ENABLE (1<<1)
#define FCR 2                 // FIFO control register
#define FCR_FIFO_ENABLE (1<<0)
#define FCR_FIFO_CLEAR (3<<1) // clear the content of the two FIFOs
#define ISR 2                 // interrupt status register
#define LCR 3                 // line control register
#define LCR_EIGHT_BITS (3<<0)
#define LCR_BAUD_LATCH (1<<7) // special mode to set baud rate
#define LSR 5                 // line status register
#define LSR_RX_READY (1<<0)   // input is waiting to be read from RHR
#define LSR_TX_IDLE (1<<5)    // THR can accept another character to send

#define ReadReg(reg) (*(Reg(reg)))

void
uartputc_sync(int c)
{
  // wait for Transmit Holding Empty to be set in LSR.
  // while((ReadReg(LSR) & LSR_TX_IDLE) == 0)
  //   ;
  WriteReg(0x000, c);
  // WriteUART0Reg(0x000, c);
}

void initclock() {
    // WriteReg(0x004, (1<<22));
    // WriteReg(0x004, (1<<23));

    int current_value = ReadClkReg(0x004);

    current_value |= (1<<18);  // Set bit 22 to 1
    current_value |= (1<<19);  // Set bit 22 to 1
                               //
    WriteClkReg(0x004, current_value);  // Write it back
}

int main() {
    initclock();

    uartinit();

    int threshold = 0x1000000;

    // state of the led
    int toggle = 0;
    int i = 0;

    while (1) {
        uartputc_sync('v');

        if (i != threshold) {
            i += 1;
            continue;
        }

        if (toggle == 1) {
            __asm__(
                "li t0, 0x03022000\n\t"
                "li t1, 1<<24\n\t"
                "sw t1, (t0)\n\t"
            );

            // next state to go to
            toggle = 0;
        } else {
            __asm__(
                "li t0, 0x03022000\n\t"
                "li t1, 0<<24\n\t"
                "sw t1, (t0)\n\t"
            );

            // next state to go to
            toggle = 1;
        }
        i = 0;
    }

    return 0;
}
    // disable interrupts.
  // WriteReg(IER, 0);

  // WriteReg(0x00c, LCR_BAUD_LATCH);


// init steps
// 1. Write 1 to LCR [7]. Enable and configure Divisor Latch Access
// 2. Write the corresponding configuration value to RBR_THR_DLL、IER_DLH
// register, configure the baud rate of transmission.
// 3. Write 0 to LCR [7].
// 4. Configure LCR and set corresponding UART working mode
// 5. Configure FCR and set corresponding transmit and receive FIFO threshold.
// 6. Turn off ETBEI/ERBFI in IER

void
uartinit(void)
{
  // special mode to set baud rate.
  WriteReg(0x00c, LCR_BAUD_LATCH);
  WriteReg(LCR, LCR_BAUD_LATCH);

  // LSB for baud rate of 38.4K.
  WriteReg(0x000, 0x03);

  // MSB for baud rate of 38.4K.
  WriteReg(0x004, 0x00);

  // leave set-baud mode
  WriteReg(0x00c, (0<<7));

  // and set word length to 8 bits, no parity.
  WriteReg(0x00c, LCR_EIGHT_BITS);

  // reset and enable FIFOs.
  WriteReg(0x008, (0<<0) | FCR_FIFO_CLEAR);

  // enable transmit and receive interrupts.
  WriteReg(0x004, IER_TX_ENABLE | IER_RX_ENABLE);
}
