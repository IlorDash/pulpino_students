#ifndef _FFT_H_
#define _FFT_H_

struct complex
{
  int16_t im;
  int16_t re;
}; //__attribute__ ((aligned (4)));


/*
void print_complex (struct complex in);
void print_complex_fixp (struct complex in);
*/


// ======== Software routines ========


void complex_add(struct complex in1,
                 struct complex in2,
                 struct complex* out);

void complex_sub(struct complex in1,
                 struct complex in2,
                 struct complex* out);

void complex_mult(struct complex in1,
                  struct complex in2,
                  struct complex* out);

void complex_mult_fixp(struct complex in1,
                         struct complex in2,
                         struct complex* out);

void buttrefly2_dit(struct complex in1,
                    struct complex in2,
                    struct complex coef,
                    struct complex* out1,
                    struct complex* out2);

uint32_t reverse_bits(uint32_t num,
                      uint32_t n_log2);

void bit_reverse_copy(uint32_t n, uint32_t n_log2,
                      struct complex* input,
                      struct complex* output);

void fft (uint32_t n, uint32_t n_log2,
          struct complex* in,
          struct complex* coef,
          struct complex* out);

uint32_t convert_struct_to_simd(struct complex x);

uint32_t fast_abs(struct complex in);



// ======== Hardware - accelerated routines ========

void buttrefly2_dit_hw(uint32_t in1,
                       uint32_t in2,
                       uint32_t coef,
                       uint32_t* out1,
                       uint32_t* out2);

uint32_t reverse_bits_hw(uint32_t num,
                      uint32_t n_log2);

void bit_reverse_copy_hw(uint32_t n, uint32_t n_log2,
                      uint32_t* input,
                      uint32_t* output);

uint32_t fast_abs_hw(uint32_t in);

void fft_hw (uint32_t n, uint32_t n_log2,
          uint32_t* in,
          uint32_t* coef,
          uint32_t* out);

#endif // _FFT_H_