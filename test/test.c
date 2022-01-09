#include <inttypes.h>
#include <assert.h>
#include <stdio.h>
#include "../ubuf.h"

int main() {

    // create ubuf
    struct ubuf _ubuf;
    struct ubuf *buf = &_ubuf;
    ub_init(buf, 1);
    
    // create test buffer
    uint8_t test_buf[] = {0xde, 0xad, 0xbe, 0xef, 0xfa, 0xce, 0xf0, 0x0d};
    uint8_t dest_buf[sizeof(test_buf)];

    // write some test values
    ub_write_u8(buf, 0xc1);
    ub_write_u16(buf, 0x306b);
    ub_write_u32(buf, 0x3567f79a);
    ub_write_u64(buf, 0xab856c944682701d);
    ub_write_u16_BE(buf, 0x2f19);
    ub_write_u32_BE(buf, 0xf29673a7);
    ub_write_u64_BE(buf, 0x29cb5f485d9f54e6);
    ub_write_float(buf, 64.126135);
    ub_write_double(buf, 81.504196);
    ub_write_float_BE(buf, 70.696124);
    ub_write_double_BE(buf, 30.517723);
    ub_write_buffer(buf, test_buf, sizeof(test_buf));

    // check write functions
    uint8_t reference[] = {0xc1, 0x6b, 0x30, 0x9a, 0xf7, 0x67, 0x35, 0x1d, 0x70, 0x82, 0x46, 0x94, 0x6c, 0x85, 0xab, 0x2f, 0x19, 0xf2, 0x96, 0x73, 0xa7, 0x29, 0xcb, 0x5f, 0x48, 0x5d, 0x9f, 0x54, 0xe6, 0x95, 0x40, 0x80, 0x42, 0x89, 0xb1, 0x4c, 0xbf, 0x44, 0x60, 0x54, 0x40, 0x42, 0x8d, 0x64, 0x6a, 0x40, 0x3e, 0x84, 0x89, 0x7e, 0x99, 0x63, 0x13, 0xde, 0xad, 0xbe, 0xef, 0xfa, 0xce, 0xf0, 0x0d};
    assert(buf->index == sizeof(reference));
    assert(!memcmp(reference, buf->buffer, buf->index));

    // reset
    buf->index = 0;

    // test read functions
    uint8_t u8;
    uint16_t u16;
    uint32_t u32;
    uint64_t u64;
    float f;
    double d;

    assert(!ub_read_u8(buf, &u8)); assert(u8 == 0xc1);
    assert(!ub_read_u16(buf, &u16)); assert(u16 == 0x306b);
    assert(!ub_read_u32(buf, &u32)); assert(u32 == 0x3567f79a);
    assert(!ub_read_u64(buf, &u64)); assert(u64 == 0xab856c944682701d);
    assert(!ub_read_u16_BE(buf, &u16)); assert(u16 == 0x2f19);
    assert(!ub_read_u32_BE(buf, &u32)); assert(u32 == 0xf29673a7);
    assert(!ub_read_u64_BE(buf, &u64)); assert(u64 == 0x29cb5f485d9f54e6);
    assert(!ub_read_float(buf, &f)); assert(f == 64.126135f);
    assert(!ub_read_double(buf, &d)); assert(d == 81.504196);
    assert(!ub_read_float_BE(buf, &f)); assert(f == 70.696124f);
    assert(!ub_read_double_BE(buf, &d)); assert(d == 30.517723);
    assert(!ub_read_buffer(buf, dest_buf, sizeof(test_buf))); assert(!memcmp(test_buf, dest_buf, sizeof(test_buf)));

    printf("final buffer size = %d, index = %d", buf->size, buf->index);
    ub_free(buf);    
    return 0;

}