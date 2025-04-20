#include "uart.h"

void 
set_uart4_pins_sd1_gpio(void) 
{
    REG_UART4_MUX_SEL = 0x4; // make sure the uart4 pins are connected to the uart4 device

    REG_IOCTRL_SD1_GPIO1 = 0x1; // Set to UART4_TX
    REG_IOCTRL_SD1_GPIO0 = 0x1; // Set to UART4_RX
}

void
uartputc_sync(int c)
{
  // wait for Transmit Holding Empty to be set in LSR.
  // while((ReadReg(LSR) & LSR_TX_IDLE) == 0)
  //   ;
  WriteReg(THR, c);

  // write to both uarts for now
  WriteUART0Reg(THR, c);
}

void initclock() {
    uint32_t clk = ReadClkReg(CLKEN1); // Read CLK_EN_1

    clk |= (1 << 22); // Enable clk_uart4
    clk |= (1 << 23); // Enable clk_apb_uart4

    WriteClkReg(CLKEN1, clk);  // Write it back
}

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
  // just make sure that the uart4 device is listening to the uart4 output
  set_uart4_pins_sd1_gpio();

  // small sleep
  for (volatile int d = 0; d < 10000; d++);

  // enable the clock for the uart 4 device
  initclock();

  // small sleep
  for (volatile int d = 0; d < 10000; d++);

  // special mode to set baud rate.
  WriteReg(LCR, LCR_BAUD_LATCH);

  // LSB for baud rate of 57.6k
  WriteReg(THR, 0x1B);

  // MSB for baud rate of 57.6k
  WriteReg(IER, 0x00);

  // leave set-baud mode and set word length to 8 bits, no parity.
  WriteReg(LCR, LCR_EIGHT_BITS);

  // reset and enable FIFOs.
  WriteReg(FCR, (0<<0) | FCR_FIFO_CLEAR);

  // enable transmit and receive interrupts.
  WriteReg(IER, IER_TX_ENABLE | IER_RX_ENABLE);

  // small sleep
  for (volatile int d = 0; d < 10000; d++);
}
