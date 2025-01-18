#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <time.h> // Add the necessary include for time()
#include "ring_buffer.h"

// Size of the audit event buffer
#define MAX_AUDIT_EVENTS 100
#define EVENT_MAX_LEN  256

// Struct to store audit event information
struct audit_event {
    uint32_t timestamp;    // Timestamp of the event
    uint32_t event_id;     // Unique event ID
    char message[EVENT_MAX_LEN]; // Event message
};

// Ring buffer to store audit events
static ring_buffer_t audit_buffer;

// Initialize the audit system
void audit_init() {
    ring_buffer_init(&audit_buffer);  // Initialize the ring buffer
}

// Get the current timestamp
uint32_t get_timestamp() {
    return (uint32_t)time(NULL);  // Use time() to get the current timestamp
}

// Log an audit event to the ring buffer
void audit_log(uint32_t event_id, const char* message) {
    struct audit_event event;
    event.timestamp = get_timestamp();
    event.event_id = event_id;
    strncpy(event.message, message, EVENT_MAX_LEN);

    // Add event to the ring buffer
    if (!ring_buffer_push(&audit_buffer, (uint8_t*)&event, sizeof(event))) {
        printf("Audit buffer is full, unable to log event.\n");
    }
}

// Print the audit log from the ring buffer
void print_audit_log() {
    struct audit_event event;
    while (!ring_buffer_is_empty(&audit_buffer)) {
        if (ring_buffer_pop(&audit_buffer, (uint8_t*)&event, sizeof(event))) {
            printf("[Audit Event %u] Timestamp: %u, Message: %s\n", 
                event.event_id, event.timestamp, event.message);
        }
    }
}
