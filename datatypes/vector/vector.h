#ifndef VECTOR_H
#define VECTOR_H

#pragma once
#include <stdlib.h>
#include <stdio.h>

typedef struct vector_t vector_t;

// create a new vector object
vector_t* vector_new();
// initialize new vector
void vector_ctr(vector_t* obj);
// delete vector
void vector_dtr(vector_t* obj);
// getter, returns size of the vector
int vector_size(vector_t* obj);
// getter, returns capacity of the vector
int vector_capacity(vector_t* obj);
// add element to the vector
void vector_push_back(vector_t* obj, const int element);
// access element at index in vector obj
int vector_at(vector_t* obj, const int index);
// access beginning of vector
int* vector_begin(vector_t* obj);
// access end of vector
int* vector_end(vector_t* obj);
// pops last element off vector
void vector_pop_back(vector_t* obj);
// erase element
void vector_erase(vector_t* obj, int* first, int* last); 


#endif // VECTOR_H