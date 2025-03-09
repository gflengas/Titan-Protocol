#include "titan_parser.h"

// List management functions
void init_packet_list(PacketList *list) {
    list->head = NULL;
    list->tail = NULL;
    list->packet_count = 0;
}

void add_packet(PacketList *list, Packet packet) {
    PacketNode *new_node = (PacketNode *)malloc(sizeof(PacketNode));
    if (!new_node) {
        return;
    }
    new_node->packet = packet;
    new_node->next = NULL;
    if (list->head == NULL) {
        list->head = new_node;
        list->tail = new_node;
    } else {
        list->tail->next = new_node;
        list->tail = new_node;
    }
    list->packet_count++;
}

void free_packet_list(PacketList *list) {
    PacketNode *current = list->head;
    while (current != NULL) {
        PacketNode *next = current->next;
        free(current);
        current = next;
    }
    list->head = NULL;
    list->packet_count = 0;
}

// Parsing function
int parse_titan_packets_from_file(const char *filename, PacketList *list) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        return -1;
    }

    char line[256];
    int successful_parses = 0;

    while (fgets(line, sizeof(line), file) != NULL) {
        // Remove newline
        size_t len = strlen(line);
        if (len > 0 && line[len - 1] == '\n') {
            line[len - 1] = '\0';
        }

        // Basic frame check
        if (line[0] != '$' || line[strlen(line) - 1] != '|') {
            continue; //skip invalid lines
        }

        // Create working buffer
        char buffer[256];
        strncpy(buffer, line + 1, sizeof(buffer) - 1);
        buffer[strlen(buffer) - 1] = '\0'; // Remove '|'

        // Initialize with zeros
        Packet packet = {0};
        char *token = strtok(buffer, ";");
        if (token == NULL) continue;
        packet.command_id = atoi(token);

        if (packet.command_id == 1) { // Ping
            token = strtok(NULL, ";");
            if (token) packet.sequence_number = atoi(token);
            token = strtok(NULL, ";");
            if (token) packet.timestamp = atoi(token) / 1000.0f; // Scale to seconds
            add_packet(list, packet);
            successful_parses++;
        }
        else if (packet.command_id == 10) { // HouseKeeping
            token = strtok(NULL, ";");
            if (token) packet.sequence_number = atoi(token);
            token = strtok(NULL, ";");
            if (token) packet.timestamp = atoi(token) / 1000.0f; // Scale to seconds
            token = strtok(NULL, ";");
            if (token) packet.payload.status = atoi(token);
            token = strtok(NULL, ";");
            if (token) packet.payload.theta = atoi(token) / 1000.0f;
            token = strtok(NULL, ";");
            if (token) packet.payload.phi = atoi(token) / 1000.0f;
            token = strtok(NULL, ";");
            if (token) packet.payload.intensity = atoi(token);
            token = strtok(NULL, ";");
            if (token) packet.payload.temperature = atoi(token) / 1000.0f;
            add_packet(list, packet);
            successful_parses++;
        }
    }

    fclose(file);
    return successful_parses;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    PacketList list;
    init_packet_list(&list);

    const char *filename = argv[1];
    int result = parse_titan_packets_from_file(filename, &list);

    if (result >= 0) {
        printf("Successfully parsed %d packets\n", result);
        printf("Total packets in list: %zu\n", list.packet_count);

        PacketNode *current = list.head;
        int packet_num = 1;
        while (current != NULL) {
            Packet *p = &current->packet;
            printf("Packet %d - Cmd: %d, Seq: %d, Time: %.3f seconds", 
                   packet_num++, p->command_id, p->sequence_number, p->timestamp);
            if (p->command_id == 10) {
                printf(", Status: %d, Theta: %.3f degrees, Phi: %.3f degrees, Int: %d, Temp: %.3f degrees",
                       p->payload.status, p->payload.theta, p->payload.phi,
                       p->payload.intensity, p->payload.temperature);
            }
            printf("\n");
            if (p->payload.status != 0)
                {
                    printf("Warning: Sensor failure detected\n");
                }
            current = current->next;
        }
    } else {
        printf("Failed to parse file\n");
        return 1;
    }

    free_packet_list(&list);
    return 0;
}