#include <stdio.h>
#include <stdlib.h>
#include <string.h> 

//typedef struct circBuffer CircularBuffer;
typedef struct circBuffer
{
    char *buffer[16];     // start of data buffer
    size_t capacity;  // max capacity of buffer -- number of string pointers buffer will contain
    size_t count;     // number of pointers in buffer
    size_t write;      // index of write
    size_t read;       // index of read
} CircularBuffer;

void FEB_circBuf_init(CircularBuffer *cb);

void FEB_circBuf_write(CircularBuffer *cb, const char *item);
void FEB_circBuf_read(CircularBuffer *cb);

