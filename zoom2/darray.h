#ifndef DARRAY_H
#define DARRAY_H

typedef struct darray darray;

darray* darray_create(int start_capacity, int item_size);
int darray_insert(darray* arr, void* data);
void* darray_get(darray* arr, unsigned int index);
int darray_remove(darray* arr, unsigned int index);
int darray_resize(darray* arr, unsigned int new_size);
void darray_destroy(darray* arr);
unsigned int darray_size(darray* arr);

#endif