
#define __riscv__
#define LED_DELAY 1000000

//#include <spi.h>
//#include <gpio.h>
//#include <uart.h>
//#include <utils.h>
//#include <pulpino.h>


int main()
{
/*
  uart_set_cfg(0, 325); // 9600 baud UART, np parity (50MHz CPU)

  uart_send("Hello world!\n", 13); // 13 is a number of chars sent: 12 + "\n" 
  uart_wait_tx_done();

  set_pin_function(11, FUNC_GPIO);
  set_gpio_pin_direction(11, DIR_OUT);

  set_gpio_pin_value(11, 0);

  while(1) {

  
    for (int i = 0; i < LED_DELAY; i++) {
      //wait some time
      #ifdef __riscv__
          asm volatile ("nop");
      #else
          asm volatile ("l.nop");
      #endif
    }

    set_gpio_pin_value(11, 1);

    for (int i = 0; i < LED_DELAY; i++) {
      //wait some time
      #ifdef __riscv__
          asm volatile ("nop");
      #else
          asm volatile ("l.nop");
      #endif
    }

    set_gpio_pin_value(11, 0);
    
  }

*/


/*
  asm volatile
  (
    "addi x2, x0, 0xa\n\t"
    "addi x3, x0, 0xb\n\t"
    "addi x4, x0, 0xc\n\t"

    "cust   x1, x2, x3, x4\n\t"
  );


*/

  int a,b,c,d,e,f,g,h;
  a = 0xabc;
  b = 0xdac;
  c = 0xcab;

  asm volatile
  (//            rd    rs1   rs2   rs3
    "btf2.t.y1   %[y], %[a], %[b], %[c]\n\t"
    : [y] "=r" (d)
    : [a] "r" (a), [b] "r" (b), [c] "r" (c) 
  );


  asm volatile
  (//            rd    rs1   rs2   rs3
    "btf2.t.y2   %[y], %[a], %[b], %[c]\n\t"
    : [y] "=r" (e)
    : [a] "r" (a), [b] "r" (b), [c] "r" (c) 
  );


  asm volatile
  (//            rd    rs1   rs2   rs3
    "btf2.f.y1   %[y], %[a], %[b], %[c]\n\t"
    : [y] "=r" (f)
    : [a] "r" (a), [b] "r" (b), [c] "r" (c) 
  );


  asm volatile
  (//            rd    rs1   rs2   rs3
    "btf2.f.y2   %[y], %[a], %[b], %[c]\n\t"
    : [y] "=r" (g)
    : [a] "r" (a), [b] "r" (b), [c] "r" (c) 
  );


  asm volatile
  (//            rd    rs1
    "btf2.abs   %[y], %[a]\n\t"
    : [y] "=r" (h)
    : [a] "r" (a) 
  );


  while(1);

}