/* util.c: common utilities */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#ifdef _WIN32
#include <SDL2/SDL.h>
#endif

void
die(const char *msg)
{
#ifdef _WIN32
  SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", msg, NULL);
#else
  fprintf(stderr, "[error] %s\n", msg);
#endif
  exit(1);
}

float
map(float val, float x0, float x1, float y0, float y1)
{
  return y0 + (y1-y0) * (val-x0)/((float)x1-x0);
}

float
log2lin(float val, float min, float max)
{
  return map(log(val), log(min), log(max), 0, 1);
}

float pow2db(float p)
{
  return 10.0f*log10f(p);
}
