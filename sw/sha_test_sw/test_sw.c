
/*#define __riscv__
#define LED_DELAY 1000000

//#include <spi.h>
#include <gpio.h>
#include <uart.h>
//#include <utils.h>
#include <pulpino.h>


uint32_t main()
{
  uart_set_cfg(0, 325); // 9600 baud UART, np parity (50MHz CPU)

  uart_send("Hello world!\n", 13); // 13 is a number of chars sent: 12 + "\n" 
  uart_wait_tx_done();

  set_pin_function(11, FUNC_GPIO);
  set_gpio_pin_direction(11, DIR_OUT);

  set_gpio_pin_value(11, 0);

  while(1) {

  
    for (uint32_t i = 0; i < LED_DELAY; i++) {
      //wait some time
      #ifdef __riscv__
          asm volatile ("nop");
      #else
          asm volatile ("l.nop");
      #endif
    }

    set_gpio_pin_value(11, 1);

    for (uint32_t i = 0; i < LED_DELAY; i++) {
      //wait some time
      #ifdef __riscv__
          asm volatile ("nop");
      #else
          asm volatile ("l.nop");
      #endif
    }

    set_gpio_pin_value(11, 0);
    
  }
}*/

//#include "stdio.h"
#define __riscv__
#define INT_W 32

#include <pulpino.h>
#include <gpio.h>
#include <stdint.h>


volatile uint32_t state[8] = { 0x6a09e667,0xbb67ae85,0x3c6ef372,0xa54ff53a,0x510e527f,0x9b05688c,0x1f83d9ab,0x5be0cd19 };
uint32_t offset = 0;

volatile uint32_t ks[64] = { 0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2 };

volatile uint32_t arr[64] = { 0x68656C6C, 0x6F20776F, 0x726C6480, 0x00000000 , 0x00000000, 0x00000000, 0x00000000, 0x00000000,
                         0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000058,
                         0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
                         0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
                         0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
                         0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
                         0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
                         0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000};
                         


uint32_t temp1_func(uint32_t h, uint32_t s1, uint32_t ch, uint32_t ks, uint32_t arr)
{
	return h + s1 + ch + ks + arr;
}


uint32_t temp2_func(uint32_t s0, uint32_t maj)
{
	return  s0 + maj;
}


/*

uint32_t r0_func(uint32_t value)
{
	return (((value >> 7) | (value << (INT_W - 7))) ^ ((value >> 18) | (value << (INT_W - 18))) ^ value >> 3);
}



uint32_t r1_func(uint32_t value)
{
	return ((value >> 17) | (value << (INT_W - 17))) ^ ((value >> 19) | (value << (INT_W - 19))) ^ value >> 10;
}



uint32_t s1_func(uint32_t value)
{
	return ((value >> 6) | (value << (INT_W - 6))) ^ ((value >> 11) | (value << (INT_W - 11))) ^ ((value >> 25) | (value << (INT_W - 25)));
}



uint32_t s0_func(uint32_t value)
{
	return ((value >> 2) | (value << (INT_W - 2))) ^ ((value >> 13) | (value << (INT_W - 13))) ^ ((value >> 22) | (value << (INT_W - 22)));
}




uint32_t ch_func(uint32_t e, uint32_t f, uint32_t g)
{
	return (e & f) ^ ((~e) & g);
}





uint32_t maj_func(uint32_t a, uint32_t b, uint32_t c)
{
	return (a & b) ^ (a & c) ^ (b & c);
}
*/









// ==================




uint32_t r0_func(uint32_t value)
{
	  uint32_t result;

  asm volatile
  (//            rd    rs1 
    "sha256.e0   %[y], %[a]\n\t"
    : [y] "=r" (result)
    : [a] "r" (value)
  );

  return result;


}




uint32_t r1_func(uint32_t value)
{
	  uint32_t result;

  asm volatile
  (//            rd    rs1 
    "sha256.e1   %[y], %[a]\n\t"
    : [y] "=r" (result)
    : [a] "r" (value)
  );

  return result;
}



uint32_t s1_func(uint32_t value)
{
	  uint32_t result;

  asm volatile
  (//            rd    rs1 
    "sha256.s1   %[y], %[a]\n\t"
    : [y] "=r" (result)
    : [a] "r" (value)
  );

  return result;


}



uint32_t s0_func(uint32_t value)
{
	  uint32_t result;

  asm volatile
  (//            rd    rs1 
    "sha256.s0   %[y], %[a]\n\t"
    : [y] "=r" (result)
    : [a] "r" (value)
  );

  return result;


}




uint32_t ch_func(uint32_t e, uint32_t f, uint32_t g)
{
  uint32_t result;

  asm volatile
  (//            rd    rs1   rs2   rs3
    "sha256.ch  %[y], %[e], %[f], %[g]\n\t"
    : [y] "=r" (result)
    : [e] "r" (e), [f] "r" (f), [g] "r" (g) 
  );

  return result;
}




uint32_t maj_func(uint32_t a, uint32_t b, uint32_t c)
{
  uint32_t result;

  asm volatile
  (//            rd    rs1   rs2   rs3
    "sha256.maj  %[y], %[a], %[b], %[c]\n\t"
    : [y] "=r" (result)
    : [a] "r" (a), [b] "r" (b), [c] "r" (c) 
  );

  return result;
}



// ��������� ����� ����� �����
/*uint32_t dectox_int(uint32_t a, uint32_t p, char *s)
{
	uint32_t num = (uint32_t)a;
	uint32_t rest = num % p;
	num /= p;
	if (num == 0)
	{
		s[0 + offset] = digit(rest); return 1;
	}
	uint32_t k = dectox_int(num, p, s);
	s[offset + k++] = digit(rest);
	return k;*/

uint32_t main()
{
	//uint32_t arr[64];
	uint32_t i = 0;
  uint32_t j = 0;
  volatile  uint32_t state_out[8];
	uint32_t temp = 0;
	uint32_t s0;
	uint32_t s1;
	uint32_t r0;
	uint32_t r1;
	//FILE *myfile;
	uint32_t a = state[0];
	uint32_t b = state[1];
	uint32_t c = state[2];
	uint32_t d = state[3];
	uint32_t e = state[4];
	uint32_t f = state[5];
	uint32_t g = state[6];
	uint32_t h = state[7];
	/*uint32_t a1;;
	uint32_t b1;
	uint32_t c1;
	uint32_t d1;
	uint32_t e1;
	uint32_t f1;
	uint32_t g1;
	uint32_t h1;*/
	uint32_t ch;
	uint32_t maj;
	uint32_t temp1;
	uint32_t temp2;


  set_pin_function(11, FUNC_GPIO);
  set_gpio_pin_direction(11, DIR_OUT);

  set_gpio_pin_value(11, 0);


  set_pin_function(10, FUNC_GPIO);
  set_gpio_pin_direction(10, DIR_OUT);

  set_gpio_pin_value(10, 1);

	// file read 
	/*if ((myfile = fopen("hello.txt", "rb")) == NULL)
	{
		printf("�� ������� ������� ����");
		getchar();
		return 0;
	}


	while (i != 64) {
		fread(buffer, sizeof(buffer), 1, myfile);
		for (size_t j = 0; j < 32; j++)
		{
			temp *= 2;
			temp += (buffer[j] - '0');
		}
		arr[i] = temp;
		temp = 0;
		i++;
	}
	fclose(myfile); */

	for (/*size_t*/ i = 16; i < 64; i++)
	{
		r0 = r0_func(arr[i - 15]);
		r1 = r1_func(arr[i - 2]);
		arr[i] = arr[i - 16] + r0 + arr[i - 7] + r1;
	}

	for (/*size_t*/ j = 0; j < 64; j++)
	{
		s1 = s1_func(e);
		ch = ch_func(e, f, g);
		temp1 = temp1_func(h, s1, ch, ks[j], arr[j]);
		s0 = s0_func(a);
		maj = maj_func(a, b, c);
		temp2 = temp2_func(s0, maj);
		h = g;
		g = f;
		f = e;
		e = d + temp1;
		d = c;
		c = b;
		b = a;
		a = temp1 + temp2;
	}

	state_out[0] = a + state[0];
	state_out[1] = b + state[1];
	state_out[2] = c + state[2];
	state_out[3] = d + state[3];
	state_out[4] = e + state[4];
	state_out[5] = f + state[5];
	state_out[6] = g + state[6];
	state_out[7] = h + state[7];

 

  /*offset += dectox_int(a1, 16, out);
	offset += dectox_int(b1, 16, out);
	offset += dectox_int(c1, 16, out);
	offset += dectox_int(d1, 16, out);
	offset += dectox_int(e1, 16, out);
	offset += dectox_int(f1, 16, out);
	offset += dectox_int(g1, 16, out);
	offset += dectox_int(h1, 16, out);


 
	/*FILE *output;
	output = fopen("out.txt", "w");
	for (size_t i = 0; i < 64; i++)
	{
		fprintf(output, "%c", out[i]);
	}*/

    set_gpio_pin_value(11, 1);

	while(1);
}