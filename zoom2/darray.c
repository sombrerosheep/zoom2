#include "darray.h"

#include <stdlib.h>
#include <string.h>

struct darray {
  unsigned int capacity;
  unsigned int size;
  unsigned int item_size;
  char* array;
};

darray* darray_create(int start_capacity, int item_size) {
  darray* arr;
  if ((arr = malloc(sizeof(darray))) == 0) {
    return NULL;
  }

  if ((arr->array = malloc(item_size * start_capacity)) == 0) {
    return NULL;
  }

  arr->capacity = start_capacity;
  arr->item_size = item_size;
  arr->size = 0;

  return arr;
}

int darray_insert(darray* arr, void* data) {
  void* tmp;

  if ((arr->size + 1) > arr->capacity) {
    // resize the array
    int new_capacity = (int)(arr->capacity * 1.5);
    if ((tmp = realloc(arr->array, new_capacity * arr->item_size)) == 0) {
      return -1;
    }
    arr->array = tmp;

    // set new capacity
    arr->capacity = new_capacity;
  }

  // insert data then increase size
  memcpy(&arr->array[arr->size * arr->item_size], data, arr->item_size);
  arr->size++;

  return 0;
}

void* darray_get(darray* arr, unsigned int index) {
  if (index >= arr->size) {
    return NULL;
  }

  return &arr->array[index * arr->item_size];
}

int darray_remove(darray* arr, unsigned int index) {
  if (index >= arr->size) {
    return -1;
  }

  // if removing the last item, just subtract the size
  if (index == arr->size - 1) {
    arr->size--;
    return 0;
  }

  // copy the last index to the index being removed
  void* dst = &arr->array[index * arr->item_size];
  void* src = &arr->array[(arr->size - 1) * arr->item_size];
  memcpy(dst, src, arr->item_size);
  arr->size--;

  return 0;
}

int darray_resize(darray* arr, unsigned int new_size) {
  void* tmp;
  if (new_size == arr->size) {
    return 0;
  }

  if ((tmp = realloc(arr->array, new_size * arr->item_size)) == 0) {
    return -1;
  }

  arr->array = tmp;
  return 0;
}

void darray_destroy(darray* arr) {
  free(arr->array);
  arr->size = 0;
  arr->capacity = 0;
  arr->item_size = 0;
}

unsigned int darray_size(darray* arr) {
  return arr->size;
}