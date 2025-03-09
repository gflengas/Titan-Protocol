#ifndef TITAN_PARSER_H
#define TITAN_PARSER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

// Define the Payload structure
typedef struct {
    int status;
    float theta;
    float phi;
    int intensity;
    float temperature;
} Payload;

// Define the Packet structure
typedef struct {
    int command_id;
    int sequence_number;
    float timestamp;
    Payload payload;
} Packet;

// Define the Packet Node structure for the linked list
typedef struct PacketNode {
    Packet packet;
    struct PacketNode *next;
} PacketNode;

// Define the Packet List structure
typedef struct {
    PacketNode *head;
    PacketNode *tail;
    size_t packet_count;
} PacketList;

// Function declarations
void init_packet_list(PacketList *list);
void add_packet(PacketList *list, Packet packet);
void free_packet_list(PacketList *list);
int parse_titan_packets_from_file(const char *filename, PacketList *list);

#endif  // TITAN_PARSER_H
