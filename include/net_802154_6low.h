/*
 * net_802154_6low.h
 * 
 * This file is part of the ION kernel project.
 * Created by the lead developer of the ION kernel.
 * 
 * License: GNU General Public License v3.0
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef NET_802154_6LOW_H
#define NET_802154_6LOW_H

#include <stdint.h>

// IEEE 802.15.4 address length (64 bits)
#define IEEE802154_ADDR_LEN 8

// IEEE 802.15.4 frame types
typedef enum {
    FRAME_BEACON = 0x00, // Beacon frame
    FRAME_DATA   = 0x01, // Data frame
    FRAME_ACK    = 0x02, // Acknowledgment frame
    FRAME_MAC    = 0x03  // MAC command frame
} ieee802154_frame_type_t;

// IEEE 802.15.4 frame header structure
typedef struct {
    uint16_t frame_control; // Frame control field
    uint8_t seq_number;     // Sequence number
    uint16_t dest_pan_id;   // Destination PAN identifier
    uint8_t dest_addr[IEEE802154_ADDR_LEN]; // Destination address
    uint16_t src_pan_id;    // Source PAN identifier
    uint8_t src_addr[IEEE802154_ADDR_LEN]; // Source address
} ieee802154_frame_header_t;

// 6LoWPAN dispatch values
#define LOWPAN_IPHC_DISPATCH 0x3F // IP Header Compression (IPHC) prefix

// 6LoWPAN packet types
typedef enum {
    LOWPAN_IPV6_PACKET = 0x00, // Uncompressed IPv6 packet
    LOWPAN_COMPRESSED  = 0x01, // Compressed packet
    LOWPAN_FRAG_FIRST  = 0x02, // First fragment
    LOWPAN_FRAG_NEXT   = 0x03  // Subsequent fragment
} lowpan_packet_type_t;

// 6LoWPAN fragmentation header structure
typedef struct {
    uint16_t dispatch;      // Dispatch field
    uint16_t datagram_size; // Total datagram size
    uint8_t datagram_tag;   // Datagram tag
    uint8_t datagram_offset; // Fragment offset (for subsequent fragments)
} lowpan_frag_header_t;

// Function prototypes
void ieee802154_init(void);
void ieee802154_send_frame(const ieee802154_frame_header_t *header, const uint8_t *payload, uint16_t payload_len);
void lowpan_compress(const uint8_t *ipv6_packet, uint16_t packet_len, uint8_t *compressed_packet, uint16_t *compressed_len);

#endif // NET_802154_6LOW_H
