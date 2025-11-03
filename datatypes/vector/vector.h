#ifndef VECTOR_H
#define VECTOR_H

#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <string.h> 
#include "../connection/conn.h"

typedef struct {
	conn_t* connection;
 } conn_map;

// vector type
struct vector
{
	unsigned int size;
	unsigned int capacity;
	unsigned int word_size;
	conn_map* elements[];
};

typedef struct vector vector;

// create new vector
vector *create_vector(int word_size, conn_map* elements);


#endif // VECTOR_H