#include "FEB_CircularBuffer.h" 


//typedef struct circBuffer
//{
//    char *buffer[16];     // start of data buffer
//    size_t capacity;  // max capacity of buffer -- number of string pointers buffer will contain
//    size_t count;     // number of pointers in buffer
//    size_t write;      // index of write
//    size_t read;       // index of read
//} CircularBuffer;

// initialize buffer on heap given capacity input. Set all string pointers to NULL 
void FEB_circBuf_init(CircularBuffer *cb)
{


    for (size_t i = 0; i < 16; i++) {
        cb->buffer[i] = NULL;
    }

    cb->capacity   = 16;
    cb->count      = 0;
    cb->write      = 0;
    cb->read       = 0;
}



// Check if maximum cappacity hasn't been reached. Copy string input, item, to heap and add pointer to buffer. Increment count of pointers and write index. 
void FEB_circBuf_write(CircularBuffer *cb, const char *item)
{

    if(cb->count == cb->capacity) {
        printf("Error! No space to write.");
        return;
    }

    cb->buffer[cb->write] = strdup(item);
    cb->write = (cb->write + 1) % cb->capacity;
    cb->count++;
}

// Check if buffer isn't full. Print earliest written string then free its space on heap. Increment read index and decrement count of pointers.
void FEB_circBuf_read(CircularBuffer *cb)
{

    if(cb->count == 0){
        printf("Error! Nothing to read.");
        return;
    }

    printf("%s", cb->buffer[cb->read]);
    free(cb->buffer[cb->read]);
    cb->read = (cb->read + 1) % cb->capacity;
    cb->count--;
}

