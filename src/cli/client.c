#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/types.h>          
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string.h>

#include "common.h"

// function for establishing hello connection
int send_hello(int fd) {

    // for now our message is just "hello"
    char msg[] = "hello";

    // write to the server using the socket fd
    write(fd, msg, strlen(msg));

    // holds the server response
    char response_buf[64] = {};
    ssize_t n = read(fd, response_buf, sizeof(response_buf) - 1);

    if (n < 0) {
        perror("read response");
        close(fd);
        return STATUS_ERROR;
    }

    // return success
    printf("Server connected, msg sent.\n");
    printf("server says: %s\n", response_buf);
    close(fd);

    return STATUS_SUCCESS;
}

int main() {
    int port = 8080;
    char *host = "127.0.0.1";

    // struct for the server 
    struct sockaddr_in serverInfo = {0}; 
    serverInfo.sin_family = AF_INET;
    serverInfo.sin_addr.s_addr = inet_addr(host);
    serverInfo.sin_port = htons(port);

    // retrieve the file desciptor from the socket connection to the sever
    // and if undefined err out
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd == -1) {
        perror("socket");
        return -1;
    }

    // with the valid file descriptor attempt to connect to the server, err
    // out if the connection fails
    if (connect(fd, (struct sockaddr*)&serverInfo, sizeof(serverInfo)) == -1) {
        perror("connect");
        close(fd);
        return 0;
    } 

    // atempt to send hello message to the server
    if (send_hello(fd) != STATUS_SUCCESS) {
        return -1;
    }

    return 0;
}