

//#define __riscv__
//#define LED_DELAY 1000000

//#include <stdio.h>
#include <stdint.h>

//#include <spi.h>
#include <gpio.h>
//#include <uart.h>
//#include <utils.h>
//#include <pulpino.h>

#include <fft.h>


#define N 8
#define N_LOG2 3
#define COEF_SIZE 12


int main()
{




  set_pin_function(11, FUNC_GPIO);
  set_gpio_pin_direction(11, DIR_OUT);
  set_gpio_pin_value(11, 0);


  set_pin_function(10, FUNC_GPIO);
  set_gpio_pin_direction(10, DIR_OUT);
  set_gpio_pin_value(10, 0);

  set_pin_function(9, FUNC_GPIO);
  set_gpio_pin_direction(9, DIR_OUT);
  set_gpio_pin_value(9, 0);

    set_pin_function(8, FUNC_GPIO);
  set_gpio_pin_direction(8, DIR_OUT);
  set_gpio_pin_value(8, 0);


  uint32_t n = N;
  uint32_t n_log2 = N_LOG2;





  // Pre - calculated coef set for 8 points FFT
  struct complex coef[COEF_SIZE] = {
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


  struct complex array_in[N] = {
  {0, 100},
  {0, 200},
  {0, 300},
  {0, 400},
  {0, 500},
  {0, 600},
  {0, 700},
  {0, 800}
  };


  struct complex array_out[N];
  uint32_t array_out_abs[N];


  uint32_t simd_coef[COEF_SIZE];
  for (int i = 0; i < COEF_SIZE; ++i)
  {
    simd_coef[i] = convert_struct_to_simd(coef[i]);
  }


  uint32_t simd_array_in[N];
  for (int i = 0; i < N; ++i)
  {
    simd_array_in[i] = convert_struct_to_simd(array_in[i]);
  }


  uint32_t simd_array_out[N];
  uint32_t simd_array_out_abs[N];


  set_gpio_pin_value(10, 1);


  fft (n, n_log2,
          array_in,
          coef,
          array_out);


  for (int i = 0; i < N; ++i)
  {
    array_out_abs[i] = fast_abs(array_out[i]);
  }


  set_gpio_pin_value(9, 1);



  fft_hw (n, n_log2,
          simd_array_in,
          simd_coef,
          simd_array_out);

  for (int i = 0; i < N; ++i)
  {
    simd_array_out_abs[i] = fast_abs_hw(simd_array_out[i]);
  }



  set_gpio_pin_value(8, 1);



while(1);

}















/*





*/