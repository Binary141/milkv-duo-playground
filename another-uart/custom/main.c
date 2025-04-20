#include <stdint.h>
#include "uart.h"

__attribute__ ((aligned (16))) char stack0[4096 * 1];

void uartinit(void);

#define UART0 0x04140000L

#define UART4 0x041C0000L

#define CLOCKGENBASEADDR 0x03002000L

#define Reg(reg) ((volatile unsigned char *)(UART4 + (reg)))

#define WriteReg(reg, v) (*(Reg(reg)) = (v))
#define ReadReg(reg) (*(Reg(reg)))

#define UART0Reg(reg) ((volatile unsigned char *)(UART0 + (reg)))
#define WriteUART0Reg(reg, v) (*(UART0Reg(reg)) = (v))

// pinmux address for the pins that can use uart4
#define REG_IOCTRL_SD1_GPIO1   (*(volatile uint32_t *)0x03001084L)  // For UART4_TX
#define REG_IOCTRL_SD1_GPIO0   (*(volatile uint32_t *)0x03001088L)  // For UART4_RX

#define REG_UART4_MUX_SEL (*(volatile uint32_t *)0x0300101E4)

void 
set_uart4_pins_sd1_gpio(void) 
{
    REG_UART4_MUX_SEL = 0x4; // make sure the uart4 pins are connected to the uart4 device

    REG_IOCTRL_SD1_GPIO1 = 0x1;  // Set to UART4_TX
    REG_IOCTRL_SD1_GPIO0 = 0x1;  // Set to UART4_RX
}

void
uartputc_sync(int c)
{
  // wait for Transmit Holding Empty to be set in LSR.
  // while((ReadReg(LSR) & LSR_TX_IDLE) == 0)
  //   ;
  WriteReg(0x000, c);

  // write to both uarts for now
  WriteUART0Reg(0x000, c);
}

#define ClkReg32(reg) ((volatile uint32_t *)(CLOCKGENBASEADDR + (reg)))
#define ReadClkReg(reg) (*(ClkReg32(reg)))
#define WriteClkReg(reg, v) (*(ClkReg32(reg)) = (v))

void initclock() {
    uint32_t v = ReadClkReg(0x004);  // Read CLK_EN_1

    v |= (1 << 22);  // Enable clk_uart4
    v |= (1 << 23);  // Enable clk_apb_uart4
                     //
    WriteClkReg(0x004, v);  // Write it back
}

int main() {
    set_uart4_pins_sd1_gpio();

    // small sleep
    for (volatile int d = 0; d < 10000; d++);

    initclock();

    // small sleep
    for (volatile int d = 0; d < 10000; d++);

    uartinit();

    // small sleep
    for (volatile int d = 0; d < 10000; d++);

    int threshold = 0x1000000;

    // state of the led
    int toggle = 0;
    int i = 0;

    while (1) {
        uartputc_sync('o');

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

// init steps
// 1. Write 1 to LCR [7]. Enable and configure Divisor Latch Access
// 2. Write the corresponding configuration value to RBR_THR_DLL、IER_DLH
// register, configure the baud rate of transmission.
// 3. Write 0 to LCR [7].
// 4. Configure LCR and set corresponding UART working mode
// 5. Configure FCR and set corresponding transmit and receive FIFO threshold.
// 6. Turn off ETBEI/ERBFI in IER

#define LCR_BAUD_LATCH (1<<7) // special mode to set baud rate
#define LCR 3                 // line control register
#define IER_RX_ENABLE (1<<0)
#define IER_TX_ENABLE (1<<1)
#define FCR_FIFO_CLEAR (3<<1) // clear the content of the two FIFOs
#define LCR_EIGHT_BITS (3<<0)

void
uartinit(void)
{
  // special mode to set baud rate.
  WriteReg(0x00c, LCR_BAUD_LATCH);
  WriteReg(LCR, LCR_BAUD_LATCH);

  // LSB for baud rate of 57.6k
  WriteReg(0x000, 0x1B);

  // MSB for baud rate of 57.6k
  WriteReg(0x004, 0x00);

  // leave set-baud mode and set word length to 8 bits, no parity.
  WriteReg(0x00c, LCR_EIGHT_BITS);

  // reset and enable FIFOs.
  WriteReg(0x008, (0<<0) | FCR_FIFO_CLEAR);

  // enable transmit and receive interrupts.
  WriteReg(0x004, IER_TX_ENABLE | IER_RX_ENABLE);
}
