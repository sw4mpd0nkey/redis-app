#ifndef REDIS_H
#define REDIS_H

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>

void msg(const char *msg);
void die(const char *msg);
void die_close_connection(const char *msg, int connfd);
int32_t read_full(int fd, char *buf, size_t n);
int32_t write_all(int fd, const char *buf, size_t n);

#endif // REDIS_H