#ifndef VECTOR_H
#define VECTOR_H

#pragma once
#include <stdlib.h>
#include <stdio.h>
#include "../connection/conn.h";

// generic vector type for interbal 
typedef void* vector;
// number of elements in a vector
typedef size_t vector_size_t;
// number of bytes for a type
typedef size_t vector_type_t;
 typedef struct {
	conn_t* connection;
 } conn_map;

// vector type
typedef struct
{
	vector_size_t size;
	vector_type_t capacity;
	conn_map* elements[];
} vector_data_t;

// returns the meta data for the vector
vector_data_t* vector_get_data(vector vec);
// creates new vector
vector_data_t* vector_new();
// create a new vector instance
vector vector_create(void);
// destroy specified vector
void vector_destroy(vector vector);
// returns current size of the specified vector
vector_size_t vector_size(vector vector);
// returns current capacity of specified vector
vector_size_t vector_capacity(vector vector);
// add element to the vector
void vector_push_back(vector* vector_address, vector_type_t type_size);

///////////////////////////////////////////////////////////


// access element at index in vector obj
int vector_at(vector_data_t* obj, const int index);
// access beginning of vector
int* vector_begin(vector_data_t* obj);
// access end of vector
int* vector_end(vector_data_t* obj);
// pops last element off vector
void vector_pop_back(vector_data_t* obj);
// erase element
void vector_erase(vector_data_t* obj, int* first, int* last); 


#endif // VECTOR_H