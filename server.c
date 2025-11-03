#include "redis.h"
#include "datatypes/vector/vector.h"
#include "datatypes/connection/conn.h"

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

    // a map of all client connections, keyed by fd
    vector *fd2conn = create_vector(sizeof(int *), compare);
    // the event loop
    vector *poll_args = create_vector(sizeof(int *), compare);

    while (1) {
        
        // prepare the arguments of the poll()
        poll_args.
        // put the listening sockets in the first position
        struct pollfd pfd = {fd, POLLIN, 0};
        poll_args.push_back(pfd);
        // the rest are connection sockets
        for (Conn *conn : fd2conn) {
            if (!conn) {
                continue;
            }
            // always poll() for error
            struct pollfd pfd = {conn->fd, POLLERR, 0};
            // poll() flags from the application's intent
            if (conn->want_read) {
                pfd.events |= POLLIN;
            }
            if (conn->want_write) {
                pfd.events |= POLLOUT;
            }
            poll_args.push_back(pfd);
        }

        // wait for readiness
        int rv = poll(poll_args.data(), (nfds_t)poll_args.size(), -1);
        if (rv < 0 && errno == EINTR) {
            continue;   // not an error
        }
        if (rv < 0) {
            die("poll");
        }

        // handle the listening socket
        if (poll_args[0].revents) {
            if (Conn *conn = handle_accept(fd)) {
                // put it into the map
                if (fd2conn.size() <= (size_t)conn->fd) {
                    fd2conn.resize(conn->fd + 1);
                }
                assert(!fd2conn[conn->fd]);
                fd2conn[conn->fd] = conn;
            }
        }

        // handle connection sockets
        for (size_t i = 1; i < poll_args.size(); ++i) { // note: skip the 1st
            uint32_t ready = poll_args[i].revents;
            if (ready == 0) {
                continue;
            }

            Conn *conn = fd2conn[poll_args[i].fd];
            if (ready & POLLIN) {
                assert(conn->want_read);
                handle_read(conn);  // application logic
            }
            if (ready & POLLOUT) {
                assert(conn->want_write);
                handle_write(conn); // application logic
            }

            // close the socket from socket error or application logic
            if ((ready & POLLERR) || conn->want_close) {
                (void)close(conn->fd);
                fd2conn[conn->fd] = NULL;
                delete conn;
            }
        }   // for each connection sockets
    }   // the event loop

    return 0;
}

  int compare(const void *a,const void *b)
  {
  return *(int *)a-*(int *)b;
  }