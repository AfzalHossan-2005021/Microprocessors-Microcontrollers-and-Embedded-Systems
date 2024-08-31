/*
 * CircularBuffer.h
 *
 * Created: 3/8/2024 9:50:54 AM
 *  Author: Afzal Hossan
 */ 


#ifndef CIRCULARBUFFER_H_
#define CIRCULARBUFFER_H_


#include <stdint.h>

#ifndef CIRCULAR_BUFFER_XS
#define CB_ST uint16_t
#else
#define CB_ST uint8_t
#endif

typedef struct {
	uint16_t ir;
	uint16_t red;
} SensorReadout;

struct CircularBuffer {
    SensorReadout *buffer;
    SensorReadout *head;
    SensorReadout *tail;
    uint16_t count;
    uint16_t size;
};

typedef struct CircularBuffer CircularBuffer;

CircularBuffer* CircularBuffer_Create(uint16_t size);
void CircularBuffer_Destroy(CircularBuffer *circularBuffer);
int CircularBuffer_Unshift(CircularBuffer *circularBuffer, void *value);
int CircularBuffer_Push(CircularBuffer *circularBuffer, void *value);
void* CircularBuffer_Shift(CircularBuffer *circularBuffer);
CircularBuffer* CircularBuffer_Pop(CircularBuffer *circularBuffer);
void* CircularBuffer_First(CircularBuffer *circularBuffer);
void* CircularBuffer_Last(CircularBuffer *circularBuffer);
void* CircularBuffer_Get(CircularBuffer *circularBuffer, CB_ST index);
CB_ST CircularBuffer_Size(CircularBuffer *circularBuffer);
CB_ST CircularBuffer_Available(CircularBuffer *circularBuffer);
CB_ST CircularBuffer_Capacity(CircularBuffer *circularBuffer);
int CircularBuffer_IsEmpty(CircularBuffer *circularBuffer);
int CircularBuffer_IsFull(CircularBuffer *circularBuffer);
void CircularBuffer_Clear(CircularBuffer *circularBuffer);

#ifdef CIRCULAR_BUFFER_DEBUG
#include <stdio.h>
void CircularBuffer_Debug(CircularBuffer *circularBuffer);
void CircularBuffer_DebugFn(CircularBuffer *circularBuffer, void (*printFunction)(void *));
#endif


#endif /* CIRCULARBUFFER_H_ */