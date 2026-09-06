#include <assert.h>
#include <complex.h>
#include <stdint.h>
#include <string.h>
#include <fcntl.h>
#include <poll.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>

#include "common.h"

int send_response(int fd) {

    char msg_buf[64] = {};
    
    // attempt to load the message from the client into the message buff, fail on err
    ssize_t n = read(fd, msg_buf, sizeof(msg_buf) - 1);
    if (n < 0) {
        perror("read() error");
        close(fd);
        return STATUS_ERROR;
    }

    printf("client says: %s\n", msg_buf);

    char response_buf[] = "world";
    write(fd, response_buf, strlen(response_buf));

    close(fd);

    return STATUS_SUCCESS;
}

int main() {

    int listen_fd,
        opt = 1,
        port = 8080;

    struct sockaddr_in server_addr, 
                       client_addr;

    // Create listening socket and make sure the file descriptor is valid
    if ((listen_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("socket");
        return -1;
    }

    if (setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
        perror("setsockopt");
        return -1;
    }

    // Set up server address structure
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port);

    // Bind
    if (bind(listen_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("bind");
        return -1;
    }

    // Listen
    if (listen(listen_fd, 10) == -1) {
        perror("listen");
        return -1;
    }

    printf("Server listening on port %d\n", port);

    // listen loop
    while (true) {

        socklen_t addrlen = sizeof(client_addr);

        int connection_fd = accept(listen_fd, (struct sockaddr *)&client_addr, &addrlen);

        if (connection_fd < 0) {
            continue;   // error
        }

        send_response(connection_fd);
        close(connection_fd);

    }

    return 0;
}