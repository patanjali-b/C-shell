#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define MAX_CHUNK_SIZE 100
#define MAX_CHUNKS 10
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
    int server_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_addr_len = sizeof(client_addr);
    Chunk received_chunks[MAX_CHUNKS];
    int total_received_chunks = 0;

    // Create UDP socket
    server_socket = socket(AF_INET, SOCK_DGRAM, 0);
    if (server_socket == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Initialize server address structure
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    // Bind the socket to the server address
    if (bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("Socket bind failed");
        exit(EXIT_FAILURE);
    }
    int alreadyReceived[MAX_CHUNKS];
    for(int i = 0 ; i < MAX_CHUNKS ; i++) {
        alreadyReceived[i] = 0;
    }

    // Receive and process data chunks from the client
    while (total_received_chunks < MAX_CHUNKS) {
        Chunk chunk;
        // Receive a chunk from the client
        recvfrom(server_socket, &chunk, sizeof(Chunk), 0, (struct sockaddr*)&client_addr, &client_addr_len);
        printf("Received chunk with sequence number: %d\n", chunk.sequence_number);

        // Process the received chunk (for demonstration purposes)
        // ...
        if(alreadyReceived[chunk.sequence_number] == 1) {
            AckPacket ack;
            ack.sequence_number = chunk.sequence_number;
            sendto(server_socket, &ack, sizeof(AckPacket), 0, (struct sockaddr*)&client_addr, client_addr_len);
            printf("Resending ACK for chunk with sequence number: %d\n", chunk.sequence_number);
            continue;
        }
        // Store the received chunk
        received_chunks[chunk.sequence_number] = chunk;

        // Send ACK for the received chunk
        int rand = (random() % 3);
        if(rand == 0) {
            total_received_chunks++;
            continue;
        }
        AckPacket ack;
        ack.sequence_number = chunk.sequence_number;
        sendto(server_socket, &ack, sizeof(AckPacket), 0, (struct sockaddr*)&client_addr, client_addr_len);
        printf("Sent ACK for chunk with sequence number: %d\n", chunk.sequence_number);

        total_received_chunks++;
    }

    // Aggregate and display the received text
    printf("\nReceived Text: ");
    for (int i = 0; i < total_received_chunks; ++i) {
        printf("%d", received_chunks[i].sequence_number);
    }
    printf("\n");

    // Close the socket
    close(server_socket);

    return 0;
}
