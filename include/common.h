#ifndef COMMON_H
#define COMMON_H

#include <stdint.h>

#define STATUS_ERROR   -1
#define STATUS_SUCCESS 0

#define HOST_IP "127.0.0.1"
#define PORT 8080

#define PROTO_VER 1.0

typedef enum {
	MSG_HELLO_REQ,
	MSG_HELLO_RESP,
	MSG_EMPLOYEE_LIST_REQ,
	MSG_EMPLOYEE_LIST_RESP,
	MSG_EMPLOYEE_ADD_REQ,
	MSG_EMPLOYEE_ADD_RESP,
	MSG_EMPLOYEE_DEL_REQ,
	MSG_EMPLOYEE_DEL_RESP,
	MSG_ERROR,
} redis_proto_type_e;

typedef struct {
	redis_proto_type_e type;
	uint16_t len;
} redis_proto_header_t;

typedef struct {
	uint16_t proto;
} redis_proto_hello_req;

typedef struct {
	uint16_t proto;
} redis_proto_hello_resp;

#endif