
#include <stdint.h>
#include <fft.h>

/*
void print_complex (struct complex in)
{
  fprintf(stderr, "%d %d i\n", in.re, in.im);

}


void print_complex_fixp (struct complex in)
{
  fprintf(stderr, "%f %f i\n", (double)in.re/32768, (double)in.im/32768);

}
*/


// ======== Software routines ========


void complex_add(struct complex in1,
                 struct complex in2,
                 struct complex* out)
{
  out->re = in1.re + in2.re;
  out->im = in1.im + in2.im;
}



void complex_sub(struct complex in1,
                 struct complex in2,
                 struct complex* out)
{
  out->re = in1.re - in2.re;
  out->im = in1.im - in2.im;
}



void complex_mult(struct complex in1,
                  struct complex in2,
                  struct complex* out)
{
  out->re = (in1.re * in2.re) - (in1.im * in2.im);
  out->im = (in1.re * in2.im) + (in1.im * in2.re);
}


void complex_mult_fixp(struct complex in1,
                         struct complex in2,
                         struct complex* out)
{

  int16_t mult1 = (in1.re * in2.re) >> 15;
  int16_t mult2 = (in1.im * in2.im) >> 15;
  int16_t mult3 = (in1.re * in2.im) >> 15;
  int16_t mult4 = (in1.im * in2.re) >> 15;

  out->re = mult1 - mult2;
  out->im = mult3 + mult4;


}



void buttrefly2_dit(struct complex in1,
                    struct complex in2,
                    struct complex coef,
                    struct complex* out1,
                    struct complex* out2)
{
  struct complex mult_res;

  complex_mult_fixp(coef,
               in2,
               &mult_res);


  complex_add(in1,
              mult_res,
              out1);


  complex_sub(in1,
              mult_res,
              out2);

}




uint32_t reverse_bits(uint32_t num,
                      uint32_t n_log2) 
{ 
    uint32_t count = sizeof(num) * 8 - 1; 
    uint32_t reverse_num = num; 
      
    num >>= 1;  
    while(num) 
    { 
       reverse_num <<= 1;        
       reverse_num |= num & 1; 
       num >>= 1; 
       count--; 
    } 
    reverse_num <<= count; 
    
    reverse_num  = reverse_num >>= 32 - n_log2;

    return reverse_num; 
} 



void bit_reverse_copy(uint32_t n, uint32_t n_log2,
                      struct complex* input,
                      struct complex* output)
{

  for (uint32_t i = 0; i < n; ++i)
  {
    output[reverse_bits(i, n_log2)] = input[i];
  }

}




void fft (uint32_t n, uint32_t n_log2,
          struct complex* in,
          struct complex* coef,
          struct complex* out) {

  bit_reverse_copy(n, n_log2,
                   in,
                   out);

  uint32_t coef_index = 0;

  for (uint32_t s = 1; s <= n_log2; ++s)
  {
    uint32_t m = 1 << s; 

    for (uint32_t k = 0; k < n; k = k + m)
    {
      for (uint32_t j = 0; j < (m >> 1); ++j)
      {            
        struct complex x2 = out[k + j + (m >> 1)];
        struct complex x1 = out[k + j];

        struct complex y1;
        struct complex y2;

        buttrefly2_dit(x1,
                       x2,
                       coef[coef_index++],
                       &y1,
                       &y2);

        out[k + j] = y1;
        out[k + j + (m >> 1)] =  y2;
      }
    }   
  }
}


uint32_t convert_struct_to_simd(struct complex x)
{
  uint32_t im = (uint32_t)x.im & 0x0000ffff;
  uint32_t re = (uint32_t)x.re & 0x0000ffff;

  return ( (im << 16) | re );
}



uint32_t fast_abs(struct complex in){

  uint32_t re_abs;
  if (in.re < 0)
    re_abs = (uint32_t) -in.re;
  else
    re_abs = (uint32_t) in.re;


  uint32_t im_abs;
  if (in.im < 0)
    im_abs = (uint32_t) -in.im;
  else
    im_abs = (uint32_t) in.im;


  uint32_t max;
  uint32_t min;


  if(re_abs > im_abs) {
    max = re_abs;
    min = im_abs;
  }
  else {
    min = re_abs;
    max = im_abs;
  }


  uint32_t max_plus_mindiv2 = max + (min >> 1);

  return max_plus_mindiv2 - (max_plus_mindiv2 >> 4);

}



// ======== Hardware - accelerated routines ========

void buttrefly2_dit_hw(uint32_t in1,
                       uint32_t in2,
                       uint32_t coef,
                       uint32_t* out1,
                       uint32_t* out2)
{

  asm volatile
  (//            rd    rs1   rs2   rs3
    "fft.b2t.y1  %[y], %[a], %[b], %[c]\n\t"
    : [y] "=r" (*out1)
    : [a] "r" (in1), [b] "r" (in2), [c] "r" (coef) 
  );

  asm volatile
  (//            rd    rs1   rs2   rs3
    "fft.b2t.y2  %[y], %[a], %[b], %[c]\n\t"
    : [y] "=r" (*out2)
    : [a] "r" (in1), [b] "r" (in2), [c] "r" (coef) 
  );

}



uint32_t reverse_bits_hw(uint32_t num,
                      uint32_t n_log2){

  uint32_t result;

  asm volatile
  (//            rd    rs1   rs2 
    "fft.bitrev  %[y], %[a], %[b]\n\t"
    : [y] "=r" (result)
    : [a] "r" (num), [b] "r" (n_log2)
  );

  return result;

} 


void bit_reverse_copy_hw(uint32_t n, uint32_t n_log2,
                      uint32_t* input,
                      uint32_t* output)
{

  for (uint32_t i = 0; i < n; ++i)
  {
    output[reverse_bits_hw(i, n_log2)] = input[i];
  }

}



uint32_t fast_abs_hw(uint32_t in){

  uint32_t result;

  asm volatile
  (//            rd    rs1 
    "fft.fabs   %[y], %[a]\n\t"
    : [y] "=r" (result)
    : [a] "r" (in)
  );

  return result;

}


void fft_hw (uint32_t n, uint32_t n_log2,
          uint32_t* in,
          uint32_t* coef,
          uint32_t* out) {

  bit_reverse_copy_hw(n, n_log2,
                   in,
                   out);

  uint32_t coef_index = 0;

  for (uint32_t s = 1; s <= n_log2; ++s)
  {
    uint32_t m = 1 << s; 

    for (uint32_t k = 0; k < n; k = k + m)
    {
      for (uint32_t j = 0; j < (m >> 1); ++j)
      {            
        uint32_t x2 = out[k + j + (m >> 1)];
        uint32_t x1 = out[k + j];

        uint32_t y1;
        uint32_t y2;

        buttrefly2_dit_hw(x1,
                          x2,
                          coef[coef_index++],
                          &y1,
                          &y2);

        out[k + j] = y1;
        out[k + j + (m >> 1)] =  y2;
      }
    }   
  }
}
