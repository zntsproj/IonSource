#ifndef BLUETOOTH_RFCOMM_H
#define BLUETOOTH_RFCOMM_H

#include <stdint.h>
#include <stdio.h>
#include <stddef.h>
#include "kernel.h"

#define RFCOMM_DEFAULT_FREQUENCY 9600  // default
#define RFCOMM_MAX_CHANNELS 30         // maximum channels

struct rfcomm_channel {
    uint8_t id;          // channel number
    uint32_t frequency;  // channel freq
    uint8_t in_use;      // use flag
};

static struct rfcomm_channel channels[RFCOMM_MAX_CHANNELS]; // channel list

static struct rfcomm_channel *find_free_channel(void) {
    for (int i = 0; i < RFCOMM_MAX_CHANNELS; i++) {
        if (!channels[i].in_use) {
            return &channels[i];
        }
    }
    return NULL;
}

static inline void rfcomm_init(void) {
    printk("Initializing RFCOMM...\n");

    for (int i = 0; i < RFCOMM_MAX_CHANNELS; i++) {
        channels[i].id = i + 1;
        channels[i].frequency = RFCOMM_DEFAULT_FREQUENCY;
        channels[i].in_use = 0;
    }

    printk("RFCOMM initialized with %d channels\n", RFCOMM_MAX_CHANNELS);
}

// Add
static inline int rfcomm_open_channel(uint32_t frequency) {
    struct rfcomm_channel *channel = find_free_channel();
    if (!channel) {
        printk("No free RFCOMM channels available\n");
        return -1;
    }

    channel->frequency = frequency;
    channel->in_use = 1;
    printk("RFCOMM channel %d opened at %u baud\n", channel->id, frequency);
    return channel->id;
}

// Close
static inline void rfcomm_close_channel(int channel_id) {
    if (channel_id < 1 || channel_id > RFCOMM_MAX_CHANNELS) {
        printk("Invalid RFCOMM channel ID: %d\n", channel_id);
        return;
    }

    struct rfcomm_channel *channel = &channels[channel_id - 1];
    if (!channel->in_use) {
        printk("RFCOMM channel %d is not in use\n", channel_id);
        return;
    }

    channel->in_use = 0;
    printk("RFCOMM channel %d closed\n", channel_id);
}

// Get channel info
static inline void rfcomm_get_channel_info(int channel_id) {
    if (channel_id < 1 || channel_id > RFCOMM_MAX_CHANNELS) {
        printk("Invalid RFCOMM channel ID: %d\n", channel_id);
        return;
    }

    struct rfcomm_channel *channel = &channels[channel_id - 1];
    if (!channel->in_use) {
        printk("RFCOMM channel %d is not in use\n", channel_id);
        return;
    }

    printk("RFCOMM channel %d is active at %u band\n", channel_id, channel->frequency);
}

#endif // BLUETOOTH_RFCOMM_H
