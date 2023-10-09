#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define MAX_CHUNK_SIZE 100
#define MAX_CHUNKS 10
#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 12345

typedef struct {
    int sequence_number;
    char data[MAX_CHUNK_SIZE];
    int total_chunks;
    int ack_received; 
} Chunk;

typedef struct {
    int sequence_number;
} AckPacket;

int main() {
    int client_socket;
    struct sockaddr_in server_addr;
    socklen_t server_addr_len = sizeof(server_addr);
    Chunk chunks[MAX_CHUNKS];
    int total_chunks = 0;

    // Create UDP socket
    client_socket = socket(AF_INET, SOCK_DGRAM, 0);
    if (client_socket == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Initialize server address structure
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);

    // Prepare data chunks (for demonstration purposes)
    for (int i = 0; i < MAX_CHUNKS; ++i) {
        chunks[i].sequence_number = i;
        chunks[i].total_chunks = MAX_CHUNKS;
        chunks[i].ack_received = 0; // Initialize acknowledgment flag to 0 (not received)
        // ...
    }

    // Send data chunks to the server
    while (total_chunks < MAX_CHUNKS) {
        // Send the next chunk
        sendto(client_socket, &chunks[total_chunks], sizeof(Chunk), 0,
               (struct sockaddr*)&server_addr, server_addr_len);
        printf("Sent chunk with sequence number: %d\n", chunks[total_chunks].sequence_number);

        // Wait for 0.1 seconds before sending the next chunk
        usleep(100000);

        // Check for ACK
        AckPacket ack;
        if (recvfrom(client_socket, &ack, sizeof(AckPacket), MSG_DONTWAIT, NULL, NULL) != -1) {
            // Received ACK for the chunk, update acknowledgment flag
            chunks[ack.sequence_number].ack_received = 1;
            printf("Received ACK for chunk with sequence number: %d\n", ack.sequence_number);
        }

        // Retransmit previous chunks whose ACK is not received yet
        for (int i = 0; i < total_chunks; ++i) {
            if (chunks[i].ack_received == 0) {
                printf("Retransmitting chunk with sequence number: %d\n", chunks[i].sequence_number);
                sendto(client_socket, &chunks[i], sizeof(Chunk), 0,
                       (struct sockaddr*)&server_addr, server_addr_len);
            }
        }

        total_chunks++;
    }

    // Close the socket
    close(client_socket);

    return 0;
}
