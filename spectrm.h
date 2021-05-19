/* spectrm.h: main module for spectrm */
#ifndef SPECTRM_H
#define SPECTRM_H

#include <fftw3.h>

typedef float sample_t;

/* dft block size */
#define N 2048
#define FFT_SIZE (N*sizeof(sample_t))

#define RB_SIZE (1<<16)

enum {
  scale = 2, fps = 60,
  padx = 50, pady = 40,
  /* height and width of inner frame */
  height = 140, width = 256
};

typedef struct spectrm_t {
  /* fftw */
  sample_t *fft_in;
  fftwf_complex *fft_out;
  fftwf_plan fft_plan;

  /* buffers */
  struct frmbuf *fb;
  struct ringbuf *rb;

  /* internal parameters */
  char nyq_str[16];
  float sr;
} spectrm_t;

#endif
