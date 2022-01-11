# ubuf

ubuf is a simple interface for reading/writing binary data. It handles automatically expanding the buffer and byte order for you, but that's about it.

## Example

```
// create buffer
struct ubuf buf;
ub_init(&buf, 1024);

// write some stuff to it
ub_write_u32(&buf, 0xdeadbeef);

// read it back
buf.index = 0;
uint32_t value;
ub_read_u32(&buf, &value);

// close
ub_free(&buf);
```
