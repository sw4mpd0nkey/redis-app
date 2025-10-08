#include "redis.h"


#include <string.h>
#include <errno.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <assert.h>

extern const uint32_t K_MAX_MSG;

static int32_t process_request(int connfd) {

    // header has four bytes
    char rbuf[4 + K_MAX_MSG + 1];
    errno = 0;
    int32_t err = read_full(connfd, rbuf, 4);
    if(err) {
        if(errno == 0) {
            msg("EOF");
        } else {
            msg("read() error");
        }
        
        return err;
    }

    uint32_t len = 0;
    // assumes little endian
    memcpy(&len, rbuf, 4);
    if (len > K_MAX_MSG) {
        msg("too long");
        return -1;
    }

    // process request body
    err = read_full(connfd, &rbuf[4], len);
    if (err) {
        msg("read() error");
        return err;
    }

    // do something
    rbuf[4 + len] = '\0';
    printf("client says: %s\n", &rbuf[4]);

    return 0;
}

static int32_t process_response(int connfd) {
    // reply using the same protocol
    const char reply[] = "world";
    char wbuf[4 + sizeof(reply)];
    uint32_t len = (uint32_t)strlen(reply);

    memcpy(wbuf, &len, 4);
    memcpy(&wbuf[4], reply, len);
    return write_all(connfd, wbuf, 4 + len);
}


int main() {
    // The AF_INET is for IPv4, use AF_INET6 for IPv6 or dual-stack socket
    // The SOCK_STREAM is for TCP. 
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    // used for tthe connections while loop below
    int run = 1;
    
    if (fd < 0) {
        die("socket()");
    }

    // this is needed for most server applications
    int val = 1;
    // The setsockopt() call is used to configure various aspects of a socket. 
    // This particular call enables the SO_REUSEADDR option
    setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(val));

    // bind
    struct sockaddr_in addr = {};
    addr.sin_family = AF_INET;
    // listening port 1234
    addr.sin_port = ntohs(1234);
    // wildcard address 0.0.0.0
    addr.sin_addr.s_addr = ntohl(0);    
    int rv = bind(fd, (const struct sockaddr *)&addr, sizeof(addr));
    if (rv) {
        die("bind()");
    }

    // listen
    rv = listen(fd, SOMAXCONN);
    if (rv) {
        die("listen()");
    }

    // Loop for each connection and do something with them.
    while (run) {

        // accept new connection
        struct sockaddr_in client_addr = {};
        socklen_t addrlen = sizeof(client_addr);

        int connfd = accept(fd, (struct sockaddr *)&client_addr, &addrlen);
        if (connfd < 0) {
            continue;   // error
        }

        while(1) {
            int32_t err = process_request(connfd);
            if(err) {
                die_close_connection("process_request()", connfd);
            } else {
                int32_t err2 = process_response(connfd);
                if(err2) {
                    die_close_connection("process_response()", connfd);
                }
            }
        }
        close(connfd);
    }

    return 0;
}