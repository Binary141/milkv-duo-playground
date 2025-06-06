#ifndef _UART_H_
#define _UART_H_

#include <stdint.h>

#define UART0 0x04140000L

#define UART0Reg(reg) ((volatile unsigned char *)(UART0 + (reg)))
#define WriteUART0Reg(reg, v) (*(UART0Reg(reg)) = (v))

#define UART4 0x041C0000L

#define CLOCKGENBASEADDR 0x03002000L

#define CLKEN1 0x004

#define ClkReg32(reg) ((volatile uint32_t *)(CLOCKGENBASEADDR + (reg)))
#define ReadClkReg(reg) (*(ClkReg32(reg)))
#define WriteClkReg(reg, v) (*(ClkReg32(reg)) = (v))

#define Reg(reg) ((volatile unsigned char *)(UART4 + (reg)))

#define WriteReg(reg, v) (*(Reg(reg)) = (v))
#define ReadReg(reg) (*(Reg(reg)))

#define WriteReg8(reg, val) (*(volatile uint8_t *)(UART4 + (reg)) = (val))
#define ReadReg8(reg) (*(volatile uint8_t *)(UART4 + (reg)))

// pinmux address for the pins that can use uart4
#define REG_IOCTRL_SD1_GPIO1   (*(volatile uint32_t *)0x03001084L)  // For UART4_TX
#define REG_IOCTRL_SD1_GPIO0   (*(volatile uint32_t *)0x03001088L)  // For UART4_RX

#define REG_UART4_MUX_SEL (*(volatile uint32_t *)0x0300101E4)

#define LCR_BAUD_LATCH (1<<7) // special mode to set baud rate
#define LCR 0x00c                 // line control register
#define LSR 0x014                 // line control register
#define THR 0x000                 // line control register
#define IER 0x004                 // line control register
#define IIR 0x008                 // line control register
#define FCR 0x008                 // line control register
#define SRR 0x088                 // software reset register
#define USR 0x07c                 // UART Status Register
#define MCR 0x010                 // Modem Control Register
#define DLL 0x000                 // Modem Control Register
#define DLH 0x004                 // Modem Control Register
#define IER_RX_ENABLE (1<<0)
#define IER_TX_ENABLE (1<<1)
#define FCR_FIFO_CLEAR (3<<1) // clear the content of the two FIFOs
#define LCR_EIGHT_BITS (3<<0)
#define LSR_TX_IDLE (1<<5)

void uartinit(void);

void set_uart4_pins_sd1_gpio(void);

void uartputc_sync(int c);

void initclock();

void print(const char*);

#endif
