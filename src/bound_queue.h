// This is free and unencumbered software released into the public domain.

// Anyone is free to copy, modify, publish, use, compile, sell, or
// distribute this software, either in source code form or as a compiled
// binary, for any purpose, commercial or non-commercial, and by any
// means.

// In jurisdictions that recognize copyright laws, the author or authors
// of this software dedicate any and all copyright interest in the
// software to the public domain. We make this dedication for the benefit
// of the public at large and to the detriment of our heirs and
// successors. We intend this dedication to be an overt act of
// relinquishment in perpetuity of all present and future rights to this
// software under copyright law.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
// IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
// OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
// OTHER DEALINGS IN THE SOFTWARE.

// For more information, please refer to <http://unlicense.org/>

#ifndef __BOUND_QUEUE_INCLUDED__
#define __BOUND_QUEUE_INCLUDED__

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

typedef struct {
  size_t         size;
  size_t         mask;
  unsigned char* buffer;
  size_t         head;
  size_t         tail;
} bound_queue_t;

bool
bound_queue_init(
    bound_queue_t* queue,
    size_t size) {
  if (!((size != 0) && ((size & (~size + 1)) == size))) {
    return false;
  }
  queue->size = size;
  queue->mask = size - 1;
  queue->buffer = malloc(sizeof(unsigned char) * size + 1);
  queue->head = 0;
  queue->tail = 0;
  return true;
}

void
bound_queue_free(
    bound_queue_t* queue) {
  free(queue->buffer);
  free(queue);
}

bool
bound_queue_enqueue(
    bound_queue_t* queue,
    unsigned char  input)
{
  if (((queue->tail - (queue->head + 1)) & queue->mask) >= 1) {
    queue->buffer[queue->head & queue->mask] = input;
    queue->head = queue->head + 1;
    return true;
  }
  return false;
}

bool
bound_queue_dequeue(
    bound_queue_t* queue,
    unsigned char* output)
{
  if (((queue->head - queue->tail) & queue->mask) >= 1) {
    *output = queue->buffer[queue->tail & queue->mask];
    queue->tail = queue->tail + 1;
    return true;
  }
  return false;
}

#endif
