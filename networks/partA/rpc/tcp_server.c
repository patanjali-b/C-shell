#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main()
{
    int server_socket;
    struct sockaddr_in server_address;

    // create socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1)
    {
        perror("Error in socket");
        exit(EXIT_FAILURE);
    }

    // specify an address for the socket
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(9002);
    server_address.sin_addr.s_addr = inet_addr("127.0.0.1");

    int k = bind(server_socket, (struct sockaddr *)&server_address, sizeof(server_address));
    if (k < 0)
    {
        perror("Error in binding");
        exit(EXIT_FAILURE);
    }

    listen(server_socket, 5);

    struct sockaddr_in clientA_address, clientB_address;
    int clientA_socket, clientB_socket;
    int addrlenA = sizeof(clientA_address);
    int addrlenB = sizeof(clientB_address);

    clientA_socket = accept(server_socket, (struct sockaddr *)&clientA_address, &addrlenA);
    if (clientA_socket < 0)
    {
        perror("Error in accepting client A");
        exit(EXIT_FAILURE);
    }

    clientB_socket = accept(server_socket, (struct sockaddr *)&clientB_address, &addrlenB);
    if (clientB_socket < 0)
    {
        perror("Error in accepting client B");
        exit(EXIT_FAILURE);
    }

    char choiceA, choiceB;
    char buffer[256];
    int bytes_received;

    char playAgainA = 'Y', playAgainB = 'Y'; // Whether the clients want to play again

    while (playAgainA == 'Y' && playAgainB == 'Y')
    {
        // Receive choices from clients
        bytes_received = recv(clientA_socket, &choiceA, sizeof(choiceA), 0);
        if (bytes_received < 0)
        {
            perror("Error receiving from client A");
            exit(EXIT_FAILURE);
        }

        bytes_received = recv(clientB_socket, &choiceB, sizeof(choiceB), 0);
        if (bytes_received < 0)
        {
            perror("Error receiving from client B");
            exit(EXIT_FAILURE);
        }

        // Determine the winner and send the result back to clients
        char result;
        if (choiceA == choiceB)
        {
            result = 'D'; // Draw
        }
        else if ((choiceA == 'R' && choiceB == 'S') ||
                 (choiceA == 'S' && choiceB == 'P') ||
                 (choiceA == 'P' && choiceB == 'R'))
        {
            result = 'A'; // Client A wins
        }
        else
        {
            result = 'B'; // Client B wins
        }

        // Send the result to clients
        send(clientA_socket, &result, sizeof(result), 0);
        send(clientB_socket, &result, sizeof(result), 0);

        // Reset choices for the next round
        choiceA = '\0';
        choiceB = '\0';

        // Receive play-again choice from clients
        recv(clientA_socket, &playAgainA, sizeof(playAgainA), 0);
        recv(clientB_socket, &playAgainB, sizeof(playAgainB), 0);

        // Send play-again choice to clients
        send(clientA_socket, &playAgainB, sizeof(playAgainB), 0);
        send(clientB_socket, &playAgainA, sizeof(playAgainA), 0);
    }

    // Close sockets
    close(clientA_socket);
    close(clientB_socket);
    close(server_socket);

    return 0;
}
