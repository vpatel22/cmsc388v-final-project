/*
 * rb.h: single-producer, single-consumer lock-free ringbuffer
 *
 * NOTE: no optimization applied, use at your own risk
 */
#ifndef RB_H
#define RB_H

#include <stddef.h>
#include <stdint.h>

typedef struct ringbuf ringbuf;

/*
 * initialize ringbuffer with capacity size-1
 *
 * return: NULL on error
 */
ringbuf* rb_init(size_t size);
/*
 * clean up ringbuffer
 */
void rb_free(ringbuf *rb);

/*
 * return: # bytes available for reading
 */
size_t rb_read_space(const ringbuf *rb);
/*
 * read to buf without advancing read pointer
 *
 * return: 1 on success and 0 on error
 */
int rb_peek(const ringbuf *rb, void *buf, size_t size);
/*
 * read to buf and advance read pointer
 *
 * return: 1 on success and 0 on error
 */
int rb_read(ringbuf *rb, void *buf, size_t size);
/*
 * advance read pointer without reading
 *
 * return: 1 on success and 0 on error
 */
int rb_read_advance(ringbuf *rb, size_t size);

/*
 * return: # bytes available for writing
 */
size_t rb_write_space(const ringbuf *rb);
/*
 * copy from buf to rb and advance write pointer
 *
 * return: 1 on success and 0 on error
 */
int rb_write(ringbuf *rb, const void *buf, size_t size);

#endif
