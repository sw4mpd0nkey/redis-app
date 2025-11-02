#include <unistd.h>
#include <stdint.h>

enum {
    STATE_REQ = 0,
    STATE_RES = 1,
    STATE_END = 2,
};

typedef struct conn_t {
    int fd;
    // either STATE_REQ or STATE_RES. The state is used to decide what to 
    // do with the connection. There are 2 states for an ongoing connection. 
    // The STATE_REQ is for reading requests and the STATE_RES is for sending
    // responses.
    uint32_t state;
    uint8_t rbuf_size;
    //bufferr for writing, we need buffers for reading/writing, since in 
    // nonblocking mode, IO operations are often deferred
    size_t wbuf_size;
    size_t wbuf_sent;
    uint8_t wbuf;
} conn_t;