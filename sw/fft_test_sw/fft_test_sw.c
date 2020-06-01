

//#define __riscv__
//#define LED_DELAY 1000000

//#include <stdio.h>
#include <stdint.h>

//#include <spi.h>
//#include <gpio.h>
//#include <uart.h>
//#include <utils.h>
//#include <pulpino.h>

#include <fft.h>


int main()
{
  uint32_t n = 8;
  uint32_t n_log2 = 3;


  fprintf(stderr, "Hello world!\n");




  // Pre - calculated coef set for 8 points FFT
  struct complex coef[12] = {
    { 00000, 32767},
    { 00000, 32767},
    { 00000, 32767},
    { 00000, 32767},
    { 00000, 32767},
    { 32768, 00000},
    { 00000, 32767},
    { 32768, 00000},
    { 00000, 32767},
    { 42366, 23170},
    { 32768, 00000},
    { 42366, 42366},
  };


  struct complex array_in[8] = {
  {0, 100},
  {0, 200},
  {0, 300},
  {0, 400},
  {0, 500},
  {0, 600},
  {0, 700},
  {0, 800}
  };


  struct complex array_out[8];


  fft (n, n_log2,
          array_in,
          coef,
          array_out);




  for (int i = 0; i < n; ++i)
  {
    print_complex (array_out[i]);
  }

  //uint32_t simd_test = convert_struct_to_simd(coef[9]);


  //fprintf(stderr, "%x\n", simd_test);
  //fprintf(stderr, "%x\n", (uint16_t)coef[9].im);
  //fprintf(stderr, "%x\n", (uint16_t)coef[9].re);


  for (int i = 0; i < n; ++i)
  {
    fprintf(stderr, "%d\n", fast_abs(array_out[i]));
  }
 

}















/*





*/