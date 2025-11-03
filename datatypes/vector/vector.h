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

unsigned int get_size(vector *vector);

unsigned int get_capacity(vector *vector);

int is_empty(vector *vector);

void push_back(vector *vector, void *data);

void pop_back(vector *vec);

void delete_vector(vector *vector);

void *get(vector *vector, unsigned int index);

void *set(vector *vector, void *data, unsigned int index);

#endif // VECTOR_H