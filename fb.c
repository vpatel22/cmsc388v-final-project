/* fb.c: 1-bit monochrome framebuffer */
#include <stdlib.h>
#include <string.h>
#include "fb.h"

extern unsigned char font[256][8];

frmbuf *
fb_new(int width, int height)
{
  frmbuf *fb = calloc(1, sizeof(frmbuf));
  if (!fb)
    return NULL;

  fb->buf = calloc(width*height, sizeof(Pixel));
  if (!fb->buf) {
    free(fb);
    return NULL;
  }
  fb_clear(fb);

  fb->width = width;
  fb->height = height;
  fb->size = width * height * sizeof(Pixel);

  return fb;
}

void
fb_free(frmbuf *fb)
{
  if (!fb)
    return;
  free(fb->buf);
  free(fb);
}

void
fb_clear(frmbuf *fb)
{
  int i;
  for (i = 0; i < fb->width*fb->height; ++i)
    fb->buf[i] = WHITE;
}

void
fb_draw(frmbuf *fb, int x, int y)
{
  if (x >= 0 && y >= 0 && x < fb->width && y < fb->height)
    fb->buf[(fb->height-1-y)*fb->width + x] = BLACK;
}

void
fb_line(frmbuf *fb, int x0, int y0, int x1, int y1)
{
  /* bresenham's algorithm */
  int dx =  abs(x1-x0), sx = x0<x1 ? 1 : -1;
  int dy = -abs(y1-y0), sy = y0<y1 ? 1 : -1;
  int e = dx + dy, e2;

  while (1) {
    fb_draw(fb, x0, y0);
    if (x0==x1 && y0==y1)
      break;
    e2 = 2*e;
    if (e2 >= dy) { e += dy; x0 += sx; }
    if (e2 <= dx) { e += dx; y0 += sy; }
  }
}

void
fb_putchar(frmbuf *fb, int x, int y, char c)
{
  int cx, cy;
  unsigned char *glyph = font[(unsigned)c];

  for (cy = 0; cy < 8; ++cy)
    for (cx = 0; cx < 8; ++cx)
      if ((glyph[cy] >> (7-cx)) & 0x1)
        fb_draw(fb, x + cx, y + (7-cy));
}

void
fb_str(frmbuf *fb, int x, int y, const char *s)
{
  while (*s) {
    fb_putchar(fb, x, y, *s);
    s++; x+=8;
  }
}

void
fb_rstr(frmbuf *fb, int x, int y, const char *s)
{
  fb_str(fb, x-8*strlen(s), y, s);
}

void
fb_cstr(frmbuf *fb, int x, int y, const char *s)
{
  fb_str(fb, x-4*strlen(s), y, s);
}
