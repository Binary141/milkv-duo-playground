#include "uart.h"

__attribute__ ((aligned (16))) char stack0[4096 * 1];

int main() {
  uartinit();

  int threshold = 0x1000000;

  // state of the led
  int toggle = 0;
  int i = 0;

  while (1) {
    print("yz");
    for (volatile int i = 0; i < 1000000; i++);  // Slow down output

    // if (i != threshold) {
    //   i += 1;
    //   continue;
    // }

    // if (toggle == 1) {
    //   __asm__(
    //       "li t0, 0x03022000\n\t"
    //       "li t1, 1<<24\n\t"
    //       "sw t1, (t0)\n\t"
    //   );

    //     // next state to go to
    //   toggle = 0;
    // } else {
    //   __asm__(
    //       "li t0, 0x03022000\n\t"
    //       "li t1, 0<<24\n\t"
    //       "sw t1, (t0)\n\t"
    //   );

    //   // next state to go to
    //   toggle = 1;
    // }

    // i = 0;
  }

  return 0;
}
