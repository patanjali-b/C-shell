#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main()
{
    int client_socket;
    struct sockaddr_in server_address;

    // Create socket
    client_socket = socket(AF_INET, SOCK_DGRAM, 0);
    if (client_socket == -1)
    {
        perror("Error in socket");
        exit(EXIT_FAILURE);
    }

    // Specify server address
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(9002);
    server_address.sin_addr.s_addr = inet_addr("127.0.0.1");

    char playAgain = 'Y';

    while (playAgain == 'Y' || playAgain == 'y')
    {
        // Get user input (rock, paper, or scissors)
        char choice;
        printf("Enter your choice (R for Rock, P for Paper, S for Scissors): ");
        scanf(" %c", &choice);

        // Send user choice to server
        int bytes_sent = sendto(client_socket, &choice, sizeof(choice), 0, (struct sockaddr *)&server_address, sizeof(server_address));
        if (bytes_sent == -1)
        {
            perror("Error sending choice to server");
            exit(EXIT_FAILURE);
        }

        // Receive and print the result from the server
        char result;
        int bytes_received = recvfrom(client_socket, &result, sizeof(result), 0, NULL, NULL);
        if (bytes_received == -1)
        {
            perror("Error receiving result from server");
            exit(EXIT_FAILURE);
        }

        if (result == 'D')
        {
            printf("It's a draw!\n");
        }
        else if (result == 'W')
        {
            printf("You win!\n");
        }
        else if (result == 'L')
        {
            printf("You lose!\n");
        }
        else
        {
            printf("Error: Invalid result received from server\n");
        }

        // Ask user if they want to play again
        printf("Do you want to play again? (Y/N): ");
        scanf(" %c", &playAgain);

        // Send playAgain to server
        int playAgainSent = sendto(client_socket, &playAgain, sizeof(playAgain), 0, (struct sockaddr *)&server_address, sizeof(server_address));
        if (playAgainSent == -1)
        {
            perror("Error sending playAgain to server");
            exit(EXIT_FAILURE);
        }

        if(playAgain == 'N')
        {
            break;
        }

        choice = '\0';

        // Receive playAgain from server
        int playAgainReceivedBytes = recvfrom(client_socket, &playAgain, sizeof(playAgain), 0, NULL, NULL);
        if (playAgainReceivedBytes == -1)
        {
            perror("Error receiving playAgain from server");
            exit(EXIT_FAILURE);
        }

        if (playAgain == 'N')
        {
            printf("The other player has quit the game.\n");
            break;
        }

    }

    // Close the socket
    close(client_socket);
    return 0;
}
