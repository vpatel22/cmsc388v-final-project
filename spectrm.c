/* spectrm.c: main module of this pitch analyzer */
#include <stdlib.h>
#include <signal.h>
#include <math.h>

#include "spectrm.h"
#include "gui.h"
#include "fb.h"
#include "rb.h"
#include "audio.h"
#include "util.h"
#include "convert_freq.h"

static spectrm_t sp;

static void
fftw_init(void)
{
  sp.fft_in = fftwf_alloc_real(N);
  sp.fft_out = fftwf_alloc_complex(N/2+1);
  if (!sp.fft_in || !sp.fft_out)
    die("fail to allocate fft arrays");

  sp.fft_plan = fftwf_plan_dft_r2c_1d(N, sp.fft_in, sp.fft_out,
                                      FFTW_ESTIMATE);
  if (!sp.fft_plan)
    die("fail to create fftw plan");
}

static void
fftw_finish(void)
{
  fftwf_destroy_plan(sp.fft_plan);
  fftwf_free(sp.fft_in);
  fftwf_free(sp.fft_out);
}

static void
spectrm_init(void)
{
  int w = width + padx*2;
  int h = height + pady*2 - 1;

  sp.fb = fb_new(w, h);
  if (!sp.fb)
    die("fail to allocate framebuffer");
  sp.rb = rb_init(RB_SIZE);
  if (!sp.rb)
    die("fail to allocate ringbuffer");

  fftw_init();
  audio_init(&sp);
  gui_init(w, h, scale, fps);
}

static int
done(void)
{
  return gui_tick();
}

static void
spectrm_finish(int signum)
{
  gui_finish();
  audio_finish();
  fftw_finish();
  rb_free(sp.rb);
  fb_free(sp.fb);
  exit(signum == SIGSEGV);
}

static float
bin2freq(int k)
{
  return k * (sp.sr/N);
}

static float 
pow2db(float p)
{
  return 10.0f*log10f(p);
}

float hann(int n)
{
	return 0.5 - 0.5*cos(2*M_PI*n/N);
}

/* return 1 if needs redraw, 0 otherwise */
static int
render(void)
{
  size_t avail;
  size_t k;
  ringbuf *rb = sp.rb;
  frmbuf *fb = sp.fb;

  /* 1. read data from audio thread */
  avail = rb_read_space(rb);
  if (avail < FFT_SIZE)
    return 0;
  rb_read_advance(rb, avail - FFT_SIZE);

  if (!rb_read(rb, sp.fft_in, FFT_SIZE))
    return 0;
  
  for (int i = 0; i < N; i++) {
    sp.fft_in[i] = sp.fft_in[i] * hann(i);
  }
  /* 2. run fft */
  fftwf_execute(sp.fft_plan);

  /* 3. draw gui */
  fb_clear(fb);

  /* axis */
  fb_line(fb, padx-1, pady,        padx+width, pady);        /* bottom */
  fb_line(fb, padx-1, pady+height, padx+width, pady+height); /* top */
  fb_line(fb, padx-1,     pady, padx-1,     pady+height);    /* left */
  fb_line(fb, padx+width, pady, padx+width, pady+height);    /* right */

  /* labels */
  fb_rstr(fb, padx-4, pady, "-90db");
  fb_rstr(fb, padx-4, pady+height-7, "0db");

  fb_str(fb, padx-1, pady-12, "20Hz");
  fb_rstr(fb, padx+width, pady-12, sp.nyq_str);

  fb_cstr(fb, padx+width/2, pady/2-4, "freq");
  fb_cstr(fb, padx/2, pady+height/2-4, "mag");

  /* spectrum */

  float max_mag = 0.0, mag_freq = 0.0;

  for (k = 1; k <= N/2; ++k) { /* skip k=0 since it's at -infty */
    float norm, re, im, mag;
    int x, y;

    norm = (N%2==0 && k==N/2) ? 1.0/N : 2.0/N;
    re = sp.fft_out[k][0] * norm;
    im = sp.fft_out[k][1] * norm;
    mag = re*re+im*im;

    x = map(log2lin(bin2freq(k), 20, sp.sr/2), 0, 1, 0, width);
    y = map(pow2db(mag), -90, 0, 0, height);

    /* if freq < 20Hz, ignore it */
    if (x < 0 || y < 0)
      continue;

    if (mag > max_mag) {
      max_mag = mag;
      mag_freq = x;
    }

    fb_line(sp.fb, padx+x, pady, padx+x, pady+y);
  }

  fb_cstr(fb, padx+width/2, pady+height + 2, get_note_name_english(clamp_midi_english(f_to_midi_rounded(mag_freq))));
  return 1;
}

int
main(int argc, char *argv[])
{
  spectrm_init();

  /* make sure to clean up gui on signal */
  signal(SIGINT, spectrm_finish);
  signal(SIGTERM, spectrm_finish);
  signal(SIGSEGV, spectrm_finish);
#ifndef _WIN32
  signal(SIGQUIT, spectrm_finish);
  signal(SIGHUP, spectrm_finish);
#endif

  while (!done()) {
    if (render())
      gui_redraw(sp.fb);
  }

  spectrm_finish(0);
  return 0;
}
