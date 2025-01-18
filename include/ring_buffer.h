#ifndef RING_BUFFER_H
#define RING_BUFFER_H

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

// Size of the ring buffer (in elements)
#define BUFFER_SIZE 1024

typedef struct {
    uint8_t buffer[BUFFER_SIZE];  // Data storage
    uint32_t head;                // Head pointer
    uint32_t tail;                // Tail pointer
} ring_buffer_t;

// Initialize the ring buffer
static inline void ring_buffer_init(ring_buffer_t* rb) {
    rb->head = 0;
    rb->tail = 0;
}

// Add a block of data to the ring buffer
static inline bool ring_buffer_push(ring_buffer_t* rb, uint8_t* data, uint32_t size) {
    uint32_t next_head = (rb->head + size) % BUFFER_SIZE;
    if (next_head == rb->tail) {
        // Buffer is full
        return false;
    }
    memcpy(&rb->buffer[rb->head], data, size);
    rb->head = next_head;
    return true;
}

// Remove a block of data from the ring buffer
static inline bool ring_buffer_pop(ring_buffer_t* rb, uint8_t* data, uint32_t size) {
    if (rb->head == rb->tail) {
        // Buffer is empty
        return false;
    }
    memcpy(data, &rb->buffer[rb->tail], size);
    rb->tail = (rb->tail + size) % BUFFER_SIZE;
    return true;
}

// Check if the buffer is empty
static inline bool ring_buffer_is_empty(ring_buffer_t* rb) {
    return rb->head == rb->tail;
}

// Check if the buffer is full
static inline bool ring_buffer_is_full(ring_buffer_t* rb) {
    return ((rb->head + 1) % BUFFER_SIZE) == rb->tail;
}

#endif // RING_BUFFER_H
