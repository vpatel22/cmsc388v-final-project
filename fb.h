/* fb.h: 1-bit monochrome framebuffer */
#ifndef FB_H
#define FB_H

#include <stdint.h>

/* different backends might use different pixel format */
typedef uint32_t Pixel;

#define BLACK 0x00000000
#define WHITE 0xffffffff

typedef struct frmbuf {
  Pixel *buf;
  int width;
  int height;
  int size;
} frmbuf;

frmbuf * fb_new(int width, int height);
void fb_free(frmbuf *fb);
void fb_clear(frmbuf *fb);

/* draw pixel at (x,y)
 * where (0,0) = lower left corner */
void fb_draw(frmbuf *fb, int x, int y);
/* draw line from (x0,y0) to (x1,y1) */
void fb_line(frmbuf *fb, int x0, int y0, int x1, int y1);

/* draw ASCII character */
void fb_putchar(frmbuf *fb, int x, int y, char c);
/* left-aligned string */
void fb_str(frmbuf *fb, int x, int y, const char *s);
/* right-aligned string */
void fb_rstr(frmbuf *fb, int x, int y, const char *s);
/* centered string */
void fb_cstr(frmbuf *fb, int x, int y, const char *s);

#endif
