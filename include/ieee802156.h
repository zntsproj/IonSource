#ifndef IEEE802156_H
#define IEEE802156_H

#include <stdint.h>

#define IEEE802156_FREQUENCY 2400000000
#define IEEE802156_BANDWIDTH 10000000 

typedef struct {
    uint32_t frequency;
    uint32_t bandwidth;
    uint32_t modulation;
    uint32_t power_level;
} ieee802156_config_t;

int configure_ieee802156(ieee802156_config_t *config);

#endif
