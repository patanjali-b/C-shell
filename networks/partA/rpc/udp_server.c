#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main()
{
    int server_socket;
    struct sockaddr_in server_address, clientA_address, clientB_address;
    socklen_t addrlenA = sizeof(clientA_address), addrlenB = sizeof(clientB_address);

    // Create socket
    server_socket = socket(AF_INET, SOCK_DGRAM, 0);
    if (server_socket == -1)
    {
        perror("Error in socket");
        exit(EXIT_FAILURE);
    }

    // Specify server address
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(9002);
    server_address.sin_addr.s_addr = inet_addr("127.0.0.1");

    int k = bind(server_socket, (struct sockaddr *)&server_address, sizeof(server_address));
    if (k < 0)
    {
        perror("Error in binding");
        exit(EXIT_FAILURE);
    }

    char playAgain = 'Y';

    while (playAgain == 'Y' || playAgain == 'y')
    {
        char choiceA, choiceB, resultA, resultB;
        int bytes_received;

        // Receive choice from client A
        bytes_received = recvfrom(server_socket, &choiceA, sizeof(choiceA), 0, (struct sockaddr *)&clientA_address, &addrlenA);
        if (bytes_received == -1)
        {
            perror("Error receiving choice from client A");
            exit(EXIT_FAILURE);
        }

        // Receive choice from client B
        bytes_received = recvfrom(server_socket, &choiceB, sizeof(choiceB), 0, (struct sockaddr *)&clientB_address, &addrlenB);
        if (bytes_received == -1)
        {
            perror("Error receiving choice from client B");
            exit(EXIT_FAILURE);
        }

        // Determine the winner
        if (choiceA == choiceB)
        {
            resultA = 'D'; // Draw
            resultB = 'D';
        }
        else if ((choiceA == 'R' && choiceB == 'S') || (choiceA == 'S' && choiceB == 'P') || (choiceA == 'P' && choiceB == 'R'))
        {
            resultA = 'W'; // Client A wins
            resultB = 'L';
        }
        else
        {
            resultA = 'L'; // Client B wins
            resultB = 'W';
        }

        // Send result to both clients
        sendto(server_socket, &resultA, sizeof(resultA), 0, (struct sockaddr *)&clientA_address, addrlenA);
        sendto(server_socket, &resultB, sizeof(resultB), 0, (struct sockaddr *)&clientB_address, addrlenB);

        choiceA = choiceB = resultA = resultB = 0;

        char continueMsgA = 'Y';
        char continueMsgB = 'Y';
        // Receive playAgain from clients
        recvfrom(server_socket, &continueMsgA, sizeof(continueMsgA), 0, (struct sockaddr *)&clientA_address, &addrlenA);
        recvfrom(server_socket, &continueMsgB, sizeof(continueMsgB), 0, (struct sockaddr *)&clientB_address, &addrlenB);

        // Ask clients if they want to continue
        // sendto(server_socket, &continueMsg, sizeof(continueMsg), 0, (struct sockaddr *)&clientA_address, addrlenA);
        // sendto(server_socket, &continueMsg, sizeof(continueMsg), 0, (struct sockaddr *)&clientB_address, addrlenB);

        if(continueMsgA == 'Y' && continueMsgB == 'Y'){
            playAgain = 'Y';
        }
        else{
            playAgain = 'N';
        }

        sendto(server_socket, &playAgain, sizeof(playAgain), 0, (struct sockaddr *)&clientA_address, addrlenA);
        sendto(server_socket, &playAgain, sizeof(playAgain), 0, (struct sockaddr *)&clientB_address, addrlenB);
    }

    close(server_socket);
    return 0;
}
