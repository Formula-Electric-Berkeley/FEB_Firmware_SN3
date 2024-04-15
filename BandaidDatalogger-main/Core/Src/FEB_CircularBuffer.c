#include "FEB_CircularBuffer.h"
extern UART_HandleTypeDef huart2;



void FEB_circBuf_init(circBuffer *cb)
{
    if (cb->buffer == NULL) {
    printf("Error! Memory not allocated for buffer.");
    exit(0);
  }

  for (size_t i = 0; i < 16; i++) {
    memset(cb->buffer[i].data, 0, 8);
    cb->buffer[i].id = 0;
  }

  cb->capacity = 16;
  cb->count = 0;
  cb->write = 0;
  cb->read = 0;
}



// Check if maximum cappacity hasn't been reached. Copy string input, item, to heap and add pointer to buffer. Increment count of pointers and write index.
void FEB_circBuf_write(circBuffer *cb, uint32_t rec_id , uint8_t *rec_data)
{
  if (cb->count == cb->capacity) {
    printf("Error! No space to write.");
    return;
  }

  memcpy(cb->buffer[cb->write].data, rec_data, 8);
  cb->buffer[cb->write].id = rec_id;
  cb->write = (cb->write + 1) % cb->capacity;
  cb->count++;
}

// Check if buffer isn't full. Print earliest written string then free its space on heap. Increment read index and decrement count of pointers.
void FEB_circBuf_read(circBuffer *cb){

  char str[50]; 
  if (cb->count == 0) {
    printf("Error! Nothing to read.");
    return;
  }

  // print id
  int size_len = sprintf(str, "ID: %ld \n", cb->buffer[cb->read].id);
  HAL_UART_Transmit(&huart2, (uint8_t*) str, size_len , HAL_MAX_DELAY);
  //printf("ID: %d", cb->buffer[cb->read]->id);

  // print data
  for (int i = 0; i < 8; i++) {
    int size_len = sprintf(str, "\t Byte: %d \n", cb->buffer[cb->read].data[i]);
    HAL_UART_Transmit(&huart2, (uint8_t*) str, size_len , HAL_MAX_DELAY);
    //printf("\t Byte %d: %d\n", i, cb->buffer[cb->read]->data[i]);
  }

  memset(cb->buffer[cb->read].data, 0, 8);
  cb->buffer[cb->read].id = 0;

  cb->read = (cb->read + 1) % cb->capacity;
  cb->count--;
}


