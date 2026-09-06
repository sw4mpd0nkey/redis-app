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
#include "srvpoll.h"

#define MAX_CLIENTS 256

clientstate_t clientStates[MAX_CLIENTS] = {0};

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

void poll_loop(unsigned short port) {

	int listen_fd, 
        conn_fd, 
        freeSlot;
    
    struct sockaddr_in server_addr, 
                       client_addr;

    socklen_t client_len = sizeof(client_addr);

    struct pollfd fds[MAX_CLIENTS + 1];
    int nfds = 1; 
    int opt = 1;

    // Initialize client states
    init_clients(clientStates);

    // Create listening socket
    if ((listen_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("socket");
        return;
    }

    if (setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
        perror("setsockopt");
        return;
    }

    // Set up server address structure
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port);

    // Bind
    if (bind(listen_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("bind");
        return;
    }

    // Listen
    if (listen(listen_fd, 10) == -1) {
        perror("listen");
        return;
    }

    printf("Server listening on port %d\n", PORT);

    memset(fds, 0, sizeof(fds));
    fds[0].fd = listen_fd;
    fds[0].events = POLLIN;
    nfds = 1;

    while (1) {

        int ii = 1;

        for (int i = 0; i < MAX_CLIENTS; i++) {
            if (clientStates[i].fd != -1) {
                fds[ii].fd = clientStates[i].fd; // Offset by 1 for listen_fd
                fds[ii].events = POLLIN;
                ii++;
            }   
        }

        // Wait for an event on one of the sockets
        int n_events = poll(fds, nfds, -1); // -1 means no timeout
        if (n_events == -1) {
            perror("poll");
            return;
        }

        // Check for new connections
        if (fds[0].revents & POLLIN) {
            if ((conn_fd = accept(listen_fd, (struct sockaddr *)&client_addr, &client_len)) == -1) {
                perror("accept");
                continue;
            }

            printf("New connection from %s:%d\n",
            	inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

            freeSlot = find_free_slot(clientStates);
            if (freeSlot == -1) {
                printf("Server full: closing new connection\n");
                close(conn_fd);
            } else {
                clientStates[freeSlot].fd = conn_fd;
                clientStates[freeSlot].state = STATE_HELLO;
                nfds++;
                printf("Slot %d has fd %d\n", freeSlot, clientStates[freeSlot].fd);
            }

            n_events--;
        }

        // Check each client for read/write activity
        for (int i = 1; i <= nfds && n_events > 0; i++) { // Start from 1 to skip the listen_fd
            if (fds[i].revents & POLLIN) {
                n_events--;

                int fd = fds[i].fd;
                int slot = find_slot_by_fd(clientStates, fd);
                ssize_t bytes_read = read(fd, &clientStates[slot].buffer, sizeof(clientStates[slot].buffer));
                if (bytes_read <= 0) {
                    // Connection closed or error
                    close(fd);

                    if (slot != -1) {
                        clientStates[slot].fd = -1; // Free up the slot
                        clientStates[slot].state = STATE_DISCONNECTED;
                        printf("Client disconnected\n");
                        nfds--;
                    }
                } else {
                    handle_client_fsm(&clientStates[slot]);
                }
            }
        }
    }
}


int main() {

    int port = 8080;

    poll_loop(port);

    return 0;
}