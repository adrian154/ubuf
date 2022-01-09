#include "ubuf.h"

#define MAYBE_EXPAND(ubuf, bytes_to_write)               \
    if(ubuf->index + bytes_to_write > ubuf->size) {       \
        size_t target_size = ubuf->size;                   \
        do {                                                \
            target_size *= 2;                                \
        } while(target_size < ubuf->index + bytes_to_write);  \
        uint8_t *new_buf = realloc(ubuf->buffer, target_size); \
        if(new_buf == NULL) {                                   \
            return -1;                                           \
        }                                                         \
        ubuf->buffer = new_buf;                                    \
        ubuf->size = target_size;                                   \
    }

#define CHECK_SIZE(ubuf, bytes_to_read)           \
    if(ubuf->index + bytes_to_read > ubuf->size) { \
        return -1;                                  \
    }

int ub_init(struct ubuf *ubuf, size_t initial_size) {
    
    assert(ubuf != NULL);
    if((ubuf->buffer = malloc(initial_size)) == NULL) {
        return -1;
    }

    ubuf->size = initial_size;
    ubuf->index = 0;
    return 0;

}

void ub_free(struct ubuf *ubuf) {
    free(ubuf->buffer);
}

int ub_read_u8(struct ubuf *ubuf, uint8_t *dest) {
    CHECK_SIZE(ubuf, 1);
    *dest = ubuf->buffer[ubuf->index++];
    return 0;
}

int ub_read_u16(struct ubuf *ubuf, uint16_t *dest) {
    CHECK_SIZE(ubuf, 2);
    *dest = (uint16_t)ubuf->buffer[ubuf->index] |
            (uint16_t)ubuf->buffer[ubuf->index + 1] << 8;
    ubuf->index += 2;
    return 0;
}

int ub_read_u32(struct ubuf *ubuf, uint32_t *dest) {
    CHECK_SIZE(ubuf, 4);
    *dest = (uint32_t)ubuf->buffer[ubuf->index] |
            (uint32_t)ubuf->buffer[ubuf->index + 1] << 8 |
            (uint32_t)ubuf->buffer[ubuf->index + 2] << 16 |
            (uint32_t)ubuf->buffer[ubuf->index + 3] << 24;
    ubuf->index += 4;
    return 0;
}

int ub_read_u64(struct ubuf *ubuf, uint64_t *dest) {
    CHECK_SIZE(ubuf, 8);
    *dest = (uint64_t)ubuf->buffer[ubuf->index] |
            (uint64_t)ubuf->buffer[ubuf->index + 1] << 8 |
            (uint64_t)ubuf->buffer[ubuf->index + 2] << 16 |
            (uint64_t)ubuf->buffer[ubuf->index + 3] << 24 |
            (uint64_t)ubuf->buffer[ubuf->index + 4] << 32 |
            (uint64_t)ubuf->buffer[ubuf->index + 5] << 40 |
            (uint64_t)ubuf->buffer[ubuf->index + 6] << 48 |
            (uint64_t)ubuf->buffer[ubuf->index + 7] << 56;
    ubuf->index += 8;
    return 0;
}

int ub_read_u16_BE(struct ubuf *ubuf, uint16_t *dest) {
    CHECK_SIZE(ubuf, 2);
    *dest = (uint16_t)ubuf->buffer[ubuf->index] << 8 |
            (uint16_t)ubuf->buffer[ubuf->index + 1];
    ubuf->index += 2;
    return 0;
}

int ub_read_u32_BE(struct ubuf *ubuf, uint32_t *dest) {
    CHECK_SIZE(ubuf, 4);
    *dest = (uint32_t)ubuf->buffer[ubuf->index] << 24 |
            (uint32_t)ubuf->buffer[ubuf->index + 1] << 16 |
            (uint32_t)ubuf->buffer[ubuf->index + 2] << 8 |
            (uint32_t)ubuf->buffer[ubuf->index + 3];
    ubuf->index += 4;
    return 0;
}

int ub_read_u64_BE(struct ubuf *ubuf, uint64_t *dest) {
    CHECK_SIZE(ubuf, 8);
    *dest = (uint64_t)ubuf->buffer[ubuf->index] << 56 |
            (uint64_t)ubuf->buffer[ubuf->index + 1] << 48 |
            (uint64_t)ubuf->buffer[ubuf->index + 2] << 40 |
            (uint64_t)ubuf->buffer[ubuf->index + 3] << 32 |
            (uint64_t)ubuf->buffer[ubuf->index + 4] << 24 |
            (uint64_t)ubuf->buffer[ubuf->index + 5] << 16 |
            (uint64_t)ubuf->buffer[ubuf->index + 6] << 8 |
            (uint64_t)ubuf->buffer[ubuf->index + 7];
    ubuf->index += 8;
    return 0;
}

int ub_read_float(struct ubuf *ubuf, float *dest) {
    uint32_t value;
    if(ub_read_u32(ubuf, &value)) {
        return -1;
    }
    *dest = *(float *)&value;
    return 0;
}

int ub_read_double(struct ubuf *ubuf, double *dest) {
    uint64_t value;
    if(ub_read_u64(ubuf, &value)) {
        return -1;
    }
    *dest = *(double *)&value;
    return 0;
}

int ub_read_float_BE(struct ubuf *ubuf, float *dest) {
    uint32_t value;
    if(ub_read_u32_BE(ubuf, &value)) {
        return -1;
    }
    *dest = *(float *)&value;
    return 0;
}

int ub_read_double_BE(struct ubuf *ubuf, double *dest) {
    uint64_t value;
    if(ub_read_u64_BE(ubuf, &value)) {
        return -1;
    }
    *dest = *(double *)&value;
    return 0;
}

int ub_read_buffer(struct ubuf *ubuf, void *dest, size_t count) {
    CHECK_SIZE(ubuf, count);
    memcpy(dest, ubuf->buffer + ubuf->index, count);
    ubuf->index += count;
    return 0;
}

int ub_write_u8(struct ubuf *ubuf, uint8_t value) {
    MAYBE_EXPAND(ubuf, 1);
    ubuf->buffer[ubuf->index++] = value;
    return 0;
}

int ub_write_u16(struct ubuf *ubuf, uint16_t value) {
    MAYBE_EXPAND(ubuf, 2);
    ubuf->buffer[ubuf->index] = value & 0xff;
    ubuf->buffer[ubuf->index + 1] = value >> 8;
    ubuf->index += 2;
    return 0;
}

int ub_write_u32(struct ubuf *ubuf, uint32_t value) {
    MAYBE_EXPAND(ubuf, 4);
    ubuf->buffer[ubuf->index] = value & 0xff;
    ubuf->buffer[ubuf->index + 1] = (value >> 8) & 0xff;
    ubuf->buffer[ubuf->index + 2] = (value >> 16) & 0xff;
    ubuf->buffer[ubuf->index + 3] = value >> 24;
    ubuf->index += 4;
    return 0;
}

int ub_write_u64(struct ubuf *ubuf, uint64_t value) {
    MAYBE_EXPAND(ubuf, 8);
    ubuf->buffer[ubuf->index] = value & 0xff;
    ubuf->buffer[ubuf->index + 1] = (value >> 8) & 0xff;
    ubuf->buffer[ubuf->index + 2] = (value >> 16) & 0xff;
    ubuf->buffer[ubuf->index + 3] = (value >> 24) & 0xff;
    ubuf->buffer[ubuf->index + 4] = (value >> 32) & 0xff;
    ubuf->buffer[ubuf->index + 5] = (value >> 40) & 0xff;
    ubuf->buffer[ubuf->index + 6] = (value >> 48) & 0xff;
    ubuf->buffer[ubuf->index + 7] = value >> 56;
    ubuf->index += 8;
    return 0;
}

int ub_write_u16_BE(struct ubuf *ubuf, uint16_t value) {
    MAYBE_EXPAND(ubuf, 2);
    ubuf->buffer[ubuf->index] = value >> 8;
    ubuf->buffer[ubuf->index + 1] = value & 0xff;
    ubuf->index += 2;
    return 0;
}

int ub_write_u32_BE(struct ubuf *ubuf, uint32_t value) {
    MAYBE_EXPAND(ubuf, 4);
    ubuf->buffer[ubuf->index] = value >> 24;
    ubuf->buffer[ubuf->index + 1] = (value >> 16) & 0xff;
    ubuf->buffer[ubuf->index + 2] = (value >> 8) & 0xff;
    ubuf->buffer[ubuf->index + 3] = value & 0xff;
    ubuf->index += 4;
    return 0;
}

int ub_write_u64_BE(struct ubuf *ubuf, uint64_t value) {
    MAYBE_EXPAND(ubuf, 8);
    ubuf->buffer[ubuf->index] = value >> 56;
    ubuf->buffer[ubuf->index + 1] = (value >> 48) & 0xff;
    ubuf->buffer[ubuf->index + 2] = (value >> 40) & 0xff;
    ubuf->buffer[ubuf->index + 3] = (value >> 32) & 0xff;
    ubuf->buffer[ubuf->index + 4] = (value >> 24) & 0xff;
    ubuf->buffer[ubuf->index + 5] = (value >> 16) & 0xff;
    ubuf->buffer[ubuf->index + 6] = (value >> 8) & 0xff;
    ubuf->buffer[ubuf->index + 7] = value & 0xff;
    ubuf->index += 8;
    return 0;
}

int ub_write_float(struct ubuf *ubuf, float value) {
    return ub_write_u32(ubuf, *(uint32_t *)&value);
}

int ub_write_double(struct ubuf *ubuf, double value) {
    return ub_write_u64(ubuf, *(uint64_t *)&value);
}

int ub_write_float_BE(struct ubuf *ubuf, float value) {
    return ub_write_u32_BE(ubuf, *(uint32_t *)&value);
}

int ub_write_double_BE(struct ubuf *ubuf, double value) {
    return ub_write_u64_BE(ubuf, *(uint64_t *)&value);
}

int ub_write_buffer(struct ubuf *ubuf, void *buffer, size_t count) {
    MAYBE_EXPAND(ubuf, count);
    memcpy(ubuf->buffer + ubuf->index, buffer, count);
    ubuf->index += count;
    return 0;
}