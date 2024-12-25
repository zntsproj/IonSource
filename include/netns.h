#ifndef ION_NETNS_H
#define ION_NETNS_H

#include <stdint.h>
#include <stddef.h>

// Define the structure for a network namespace
struct netns {
    uint32_t id;          // Unique identifier for the namespace
    void *net_devices;    // Pointer to the list of network devices (e.g., interfaces)
    void *routes;         // Pointer to the routing table
    void *namespaces;     // Pointer to nested namespaces (if any)
};

// Maximum number of network namespaces
#define MAX_NETNS 16

// Static array to hold network namespaces
static struct netns netns_array[MAX_NETNS];

// Index of the currently active network namespace
static int current_netns_idx = -1;

// Function to initialize network namespaces
void netns_init(void) {
    for (int i = 0; i < MAX_NETNS; i++) {
        netns_array[i].id = 0;  // Initially, all namespaces are inactive
        netns_array[i].net_devices = NULL;
        netns_array[i].routes = NULL;
        netns_array[i].namespaces = NULL;
    }
}

// Function to create a new network namespace
int netns_create(void) {
    for (int i = 0; i < MAX_NETNS; i++) {
        if (netns_array[i].id == 0) {  // Find an available slot
            netns_array[i].id = i + 1;  // Assign a unique identifier
            return i;  // Return the index of the new namespace
        }
    }
    return -1;  // Error, if all namespaces are occupied
}

// Function to set the current active network namespace
int netns_set_current(int idx) {
    if (idx < 0 || idx >= MAX_NETNS || netns_array[idx].id == 0) {
        return -1;  // Invalid index or namespace does not exist
    }
    current_netns_idx = idx;
    return 0;  // Success
}

// Function to get the current active network namespace
struct netns *netns_get_current(void) {
    if (current_netns_idx == -1) {
        return NULL;  // No active namespace
    }
    return &netns_array[current_netns_idx];
}

// Function to remove (clear) a network namespace
void netns_remove(int idx) {
    if (idx < 0 || idx >= MAX_NETNS) {
        return;  // Invalid index
    }
    netns_array[idx].id = 0;  // Free the namespace
    netns_array[idx].net_devices = NULL;
    netns_array[idx].routes = NULL;
    netns_array[idx].namespaces = NULL;
}

#endif // ION_NETNS_H
