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

    char buf[4096] = {0};

    redis_proto_header_t *hdr = (redis_proto_header_t *)buf;
    hdr->type = MSG_HELLO_REQ;
    hdr->len = 1;

    // send the hello request with the version we speak
    redis_proto_hello_req* hello = (redis_proto_hello_req*)&hdr[1];
    hello->proto = PROTO_VER;

    hdr->type = htonl(hdr->type);
    hdr->len = htons(hdr->len);
    hello->proto = htons(hello->proto);

    // write the hello message
    write(fd, buf, sizeof(redis_proto_header_t) + sizeof(redis_proto_hello_req));
  
    // recv the response
    read(fd, buf, sizeof(buf));

    hdr->type = ntohl(hdr->type);
    hdr->len = ntohs(hdr->len);

    // handle error response
    if (hdr->type == MSG_ERROR) {
  	    printf("Protocol mismatch.\n");
  	    close(fd);
        return STATUS_ERROR;
    }

    // return success
    printf("Server connected, protocol v1.\n");
    return STATUS_SUCCESS;
}

int main() {
    int port = 8080;
    char *host = "127.0.0.1";

    // struct for the server information. htons whiches the endieness
    // of the data for transporting acress the wire 
    struct sockaddr_in serverInfo = {0}; 
    serverInfo.sin_family = AF_INET;
    serverInfo.sin_addr.s_addr = inet_addr(HOST_IP);
    serverInfo.sin_port = htons(PORT);

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

    // atempt to send hello message to the server, the MSG_HELLO_REQ starts the protocol
    // hand shake
    if (send_hello(fd) != STATUS_SUCCESS) {
        return -1;
    }

    return 0;
}