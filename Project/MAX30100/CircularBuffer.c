/*
 * CircularBuffer.c
 *
 * Created: 3/8/2024 10:10:55 AM
 *  Author: Afzal Hossan
 */ 

#include "CircularBuffer.h"
#include <stdlib.h>
#include <string.h>

CircularBuffer* CircularBuffer_Create(uint16_t size) {
	CircularBuffer *circularBuffer = (CircularBuffer*)malloc(sizeof(CircularBuffer));
	if (circularBuffer == NULL) {
		return NULL;
	}
	circularBuffer->buffer = malloc(size * sizeof(void*)); // Allocating memory for buffer elements
	if (circularBuffer->buffer == NULL) {
		free(circularBuffer);
		return NULL;
	}
	circularBuffer->head = circularBuffer->buffer;
	circularBuffer->tail = circularBuffer->buffer;
	circularBuffer->count = 0;
	circularBuffer->size = size;
	return circularBuffer;
}

void CircularBuffer_Destroy(CircularBuffer *circularBuffer) {
	if (circularBuffer != NULL) {
		free(circularBuffer->buffer);
		free(circularBuffer);
	}
}

int CircularBuffer_Unshift(CircularBuffer *circularBuffer, void *value) {
	if (circularBuffer->count == circularBuffer->size) {
		return 0; // Buffer is full
	}
	circularBuffer->count++;
	if (circularBuffer->head == circularBuffer->buffer) {
		circularBuffer->head = circularBuffer->buffer + circularBuffer->size * sizeof(void*);
	}
	circularBuffer->head = circularBuffer->head - sizeof(void*);
	memcpy(circularBuffer->head, &value, sizeof(void*));
	return 1; // Success
}

int CircularBuffer_Push(CircularBuffer *circularBuffer, void *value) {
	if (circularBuffer->count == circularBuffer->size) {
		return 0; // Buffer is full
	}
	circularBuffer->count++;
	memcpy(circularBuffer->tail, &value, sizeof(void*));
	circularBuffer->tail = circularBuffer->tail + sizeof(void*);
	if (circularBuffer->tail == circularBuffer->buffer + circularBuffer->size * sizeof(void*)) {
		circularBuffer->tail = circularBuffer->buffer;
	}
	return 1; // Success
}

void* CircularBuffer_Shift(CircularBuffer *circularBuffer) {
	if (circularBuffer->count == 0) {
		return NULL; // Buffer is empty
	}
	void *value = *(void**)circularBuffer->head;
	circularBuffer->count--;
	circularBuffer->head = circularBuffer->head + sizeof(void*);
	if (circularBuffer->head == circularBuffer->buffer + circularBuffer->size * sizeof(void*)) {
		circularBuffer->head = circularBuffer->buffer;
	}
	return value;
}

CircularBuffer* CircularBuffer_Pop(CircularBuffer *circularBuffer) {
	if (circularBuffer->count == 0) {
		return NULL; // Buffer is empty
	}
	if (circularBuffer->tail == circularBuffer->buffer) {
		circularBuffer->tail = circularBuffer->buffer + circularBuffer->size * sizeof(void*);
	}
	circularBuffer->tail = circularBuffer->tail - sizeof(void*);
	void *value = *(void**)circularBuffer->tail;
	circularBuffer->count--;
	return value;
}

void* CircularBuffer_First(CircularBuffer *circularBuffer) {
	if (circularBuffer->count == 0) {
		return NULL; // Buffer is empty
	}
	return *(void**)circularBuffer->head;
}

void* CircularBuffer_Last(CircularBuffer *circularBuffer) {
	if (circularBuffer->count == 0) {
		return NULL; // Buffer is empty
	}
	if (circularBuffer->tail == circularBuffer->buffer) {
		return *(void**)((char*)circularBuffer->buffer + (circularBuffer->size - 1) * sizeof(void*));
	}
	return *(void**)((char*)circularBuffer->tail - sizeof(void*));
}

void* CircularBuffer_Get(CircularBuffer *circularBuffer, CB_ST index) {
	if (index >= circularBuffer->count) {
		return NULL; // Index out of bounds
	}
	char *ptr = (char*)circularBuffer->head + index * sizeof(void*);
	if (ptr >= (char*)circularBuffer->buffer + circularBuffer->size * sizeof(void*)) {
		ptr -= circularBuffer->size * sizeof(void*);
	}
	return *(void**)ptr;
}

CB_ST CircularBuffer_Size(CircularBuffer *circularBuffer) {
	return circularBuffer->count;
}

CB_ST CircularBuffer_Available(CircularBuffer *circularBuffer) {
	return circularBuffer->size - circularBuffer->count;
}

CB_ST CircularBuffer_Capacity(CircularBuffer *circularBuffer) {
	return circularBuffer->size;
}

int CircularBuffer_IsEmpty(CircularBuffer *circularBuffer) {
	return circularBuffer->count == 0;
}

int CircularBuffer_IsFull(CircularBuffer *circularBuffer) {
	return circularBuffer->count == circularBuffer->size;
}

void CircularBuffer_Clear(CircularBuffer *circularBuffer) {
	circularBuffer->head = circularBuffer->buffer;
	circularBuffer->tail = circularBuffer->buffer;
	circularBuffer->count = 0;
}

#ifdef CIRCULAR_BUFFER_DEBUG
#include <stdio.h>
void CircularBuffer_Debug(CircularBuffer *circularBuffer) {
	printf("Buffer contents:\n");
	for (CB_ST i = 0; i < circularBuffer->size; i++) {
		void *value = CircularBuffer_Get(circularBuffer, i);
		printf("%p  %p\n", (void*)((char*)circularBuffer->buffer + i * sizeof(void*)), value);
	}
}

void CircularBuffer_DebugFn(CircularBuffer *circularBuffer, void (*printFunction)(void *)) {
	printf("Buffer contents:\n");
	for (CB_ST i = 0; i < circularBuffer->size; i++) {
		void *value = CircularBuffer_Get(circularBuffer, i);
		printf("%p  ", (void*)((char*)circularBuffer->buffer + i * sizeof(void*)));
		printFunction(value);
		printf("\n");
	}
}
#endif
