/*
 * rb.c: single-producer, single-consumer lock-free ringbuffer
 *
 * NOTE: no optimization applied, use at your own risk
 */
#include <stdlib.h>
#include <string.h>

#include "rb.h"

struct ringbuf {
  _Atomic size_t rp;
  _Atomic size_t wp;
  size_t size;
  void *buf;
};

ringbuf *
rb_init(size_t size)
{
  ringbuf *rb = calloc(1, sizeof(ringbuf));
  if (!rb)
    return NULL;

  rb->wp = 0;
  rb->rp = 0;
  rb->size = size;

  rb->buf = calloc(1, size);
  if (!rb->buf) {
    free(rb);
    return NULL;
  }

  return rb;
}

void
rb_free(ringbuf *rb)
{
  if (!rb)
    return;

  free(rb->buf);
  free(rb);
}

size_t
rb_read_space(const ringbuf *rb)
{
  size_t rp, wp;

  rp = rb->rp;
  wp = rb->wp;

  if (wp >= rp)
    /*
     * |-------|**********|---|
     *         rp         wp
     */
    return wp - rp;
  else
    /*
     * |*******|----------|***|
     *         wp         rp
     */
    return rb->size - (rp - wp);
}

int
rb_peek(const ringbuf *rb, void *buffer, size_t size)
{
  size_t rp;
  char *buf = buffer;
  const char *rbbuf = rb->buf;

  if (rb_read_space(rb) < size)
    return 0;

  rp = rb->rp;

  if (rp + size < rb->size) {
    memcpy(buf, rbbuf + rp, size);
  } else {
    size_t s = rb->size - rp;
    memcpy(buf, rbbuf + rp, s);
    memcpy(buf + s, rbbuf, size - s);
  }
  return 1;
}

int
rb_read(ringbuf *rb, void *buf, size_t size)
{
  if (!rb_peek(rb, buf, size))
    return 0;

  rb->rp = (rb->rp + size) % rb->size;

  return 1;
}

int
rb_read_advance(ringbuf *rb, size_t size)
{
  if (rb_read_space(rb) < size)
    return 0;
  rb->rp = (rb->rp + size) % rb->size;

  return 1;
}

size_t
rb_write_space(const ringbuf *rb)
{
  size_t rp, wp;

  rp = rb->rp;
  wp = rb->wp;

  if (rp == wp)
    return rb->size - 1;
  else if (rp > wp)
    /*
     * |-------|**********|---|
     *         wp         rp
     */
    return rp - wp - 1;
  else
    /*
     * |*******|----------|***|
     *         rp         wp
     */
    return rb->size - (wp - rp) - 1;
}

int
rb_write(ringbuf *rb, const void *buffer, size_t size)
{
  size_t wp;
  const char *buf = buffer;
  char *rbbuf = rb->buf;

  if (rb_write_space(rb) < size)
    return 0;

  wp = rb->wp;

  if (wp + size < rb->size) {
    memcpy(rbbuf + wp, buf, size);
  } else {
    size_t s = rb->size - wp;
    memcpy(rbbuf + wp, buf, s);
    memcpy(rbbuf, buf + s, size - s);
  }
  rb->wp = (wp + size) % rb->size;

  return 1;
}
