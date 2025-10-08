#include <errno.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/ip.h>

#include "redis.h"


extern const uint32_t K_MAX_MSG;


static int32_t send_request(int fd, const char *text) {
    uint32_t len = (uint32_t)strlen(text);

    if (len > K_MAX_MSG) {
        return -1;  
    }

    char wbuf[4 + K_MAX_MSG];

    // assume little endian
    memcpy(wbuf, &len, 4); 
    memcpy(&wbuf[4], text, len);

    int32_t err = (int32_t)write_all(fd, wbuf, 4 + len);
    if (err) {
        return err;
    }

    // 4 bytes header
    char rbuf[4 + K_MAX_MSG + 1];
    errno = 0;
    err = read_full(fd, rbuf, 4);
    
    if (err) {
        if (errno == 0) {
            msg("EOF");
        } else {
            msg("read() error");
        }
        return err;
    }

    memcpy(&len, rbuf, 4); // assume little endian

    if (len > K_MAX_MSG) {
        msg("too long");
        return -1;
    }

    // reply body
    err = read_full(fd, &rbuf[4], len);
    if (err) {
        msg("read() error");
        return err;
    }

    // print out the response from the server
    rbuf[4 + len] = '\0';
    printf("server says: %s\n", &rbuf[4]);
    return 0;
}

int main() {

    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd < 0) {
        die("socket()");
    }

    struct sockaddr_in addr = {};
    addr.sin_family = AF_INET;
    addr.sin_port = ntohs(1234);
    addr.sin_addr.s_addr = ntohl(INADDR_LOOPBACK);  // 127.0.0.1
    int rv = connect(fd, (const struct sockaddr *)&addr, sizeof(addr));
    if (rv) {
        die("connect");
    }

    // updating the client here to send multiple requests

    int32_t err = send_request(fd, "hello1");
    if (err) {
        goto L_DONE;
    }
    err = send_request(fd, "hello2");
    if (err) {
        goto L_DONE;
    }
    err = send_request(fd, "hello3");
    if (err) {
        goto L_DONE;
    }

L_DONE:
    close(fd);
    return 0;
}