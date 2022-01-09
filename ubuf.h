#ifndef __UBUF_H
#define __UBUF_H

#include <assert.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

struct ubuf {
    size_t size;
    size_t index;
    uint8_t *buffer;
};

// all functions return -1 on failure, 0 on success
extern int ub_init(struct ubuf *ubuf, size_t initial_size);
extern void ub_free(struct ubuf *ubuf);

// read functions
extern int ub_read_u8(struct ubuf *ubuf, uint8_t *dest);
extern int ub_read_u16(struct ubuf *ubuf, uint16_t *dest);
extern int ub_read_u32(struct ubuf *ubuf, uint32_t *dest);
extern int ub_read_u64(struct ubuf *ubuf, uint64_t *dest);
extern int ub_read_u16_BE(struct ubuf *ubuf, uint16_t *dest);
extern int ub_read_u32_BE(struct ubuf *ubuf, uint32_t *dest);
extern int ub_read_u64_BE(struct ubuf *ubuf, uint64_t *dest);
extern int ub_read_float(struct ubuf *ubuf, float *dest);
extern int ub_read_double(struct ubuf *ubuf, double *dest);
extern int ub_read_float_BE(struct ubuf *ubuf, float *dest);
extern int ub_read_double_BE(struct ubuf *ubuf, double *dest);

// write functions
extern int ub_write_u8(struct ubuf *ubuf, uint8_t value);
extern int ub_write_u16(struct ubuf *ubuf, uint16_t value);
extern int ub_write_u32(struct ubuf *ubuf, uint32_t value);
extern int ub_write_u64(struct ubuf *ubuf, uint64_t value);
extern int ub_write_u16_BE(struct ubuf *ubuf, uint16_t value);
extern int ub_write_u32_BE(struct ubuf *ubuf, uint32_t value);
extern int ub_write_u64_BE(struct ubuf *ubuf, uint64_t value);
extern int ub_write_float(struct ubuf *ubuf, float value);
extern int ub_write_double(struct ubuf *ubuf, double value);
extern int ub_write_float_BE(struct ubuf *ubuf, float value);
extern int ub_write_double_BE(struct ubuf *ubuf, double vaue);

#endif