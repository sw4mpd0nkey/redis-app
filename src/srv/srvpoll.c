#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <poll.h>

#include "common.h"
#include "srvpoll.h"

void fsm_reply_hello(clientstate_t *client, redis_proto_header_t *header) {
    header->type = htonl(MSG_HELLO_RESP);
    header->len = htons(1);
    redis_proto_hello_resp* hello = (redis_proto_hello_resp*) &header[1];
    hello->proto = htons(PROTO_VER);

    write(client->fd, header, sizeof(redis_proto_header_t) + sizeof(redis_proto_hello_resp));
}

void fsm_reply_hello_err(clientstate_t *client, redis_proto_header_t *header) {
    header->type = htonl(MSG_ERROR);
    header->len = htons(0);

    write(client->fd, header, sizeof(redis_proto_header_t));
}

void init_clients(clientstate_t* states) {
    for (int i = 0; i < MAX_CLIENTS; i++) {
        states[i].fd = -1; // -1 indicates a free slot
        states[i].state = STATE_NEW;
        memset(&states[i].buffer, '\0', BUFF_SIZE);
    }
}

int find_free_slot(clientstate_t* states) {
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (states[i].fd == -1) {
            return i;
        }
    }
    return -1; // No free slot found
}

int find_slot_by_fd(clientstate_t* states, int fd) {
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (states[i].fd == fd) {
            return i;
        }
    }
    return -1; // Not found
}


// functions that handles the finite state machine for client requests. Based on
// the clients state there are limited possible action options and this will
// verify that only allowed requests go through and processes the request
void handle_client_fsm(clientstate_t *client) {
    redis_proto_header_t *header = (redis_proto_header_t*)client->buffer; 

    // ntol translates the endieness from the wire to the host
    header->type = ntohl(header->type);
    header->len = ntohs(header->len);

    // currently the only state that is handled, in the hello state the server is waiting
    // for a hello req msg from the client and will only move to the next state when it
    // recieves it
    if (client->state == STATE_HELLO) {
        if (header->type != MSG_HELLO_REQ || header->len != 1) {
            printf("Didn't get MSG_HELLO in HELLO state...\n");
            fsm_reply_hello_err(client, header);
            return;
        }

        // request is a hello req msg, checking the protocol versions match
        // before moving forward
        redis_proto_hello_req* hello = (redis_proto_hello_req*)&header[1];
        hello->proto = ntohs(hello->proto);
        if (hello->proto != PROTO_VER) {
            printf("Protocol mismatch...\n");
            fsm_reply_hello_err(client, header);
            return;
        }

        // protocol versions match so moving state to STATE_MSG
        fsm_reply_hello(client, header);
        client->state = STATE_MSG;
        printf("Client upgraded to STATE_MSG\n");
    }

    if (client->state == STATE_MSG) {
        //TODO: Need to figure out the various state messages
    }
}