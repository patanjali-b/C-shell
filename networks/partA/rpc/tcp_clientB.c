#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main()
{
    int clientB_socket;
    struct sockaddr_in server_address;

    // Create socket
    clientB_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientB_socket == -1)
    {
        perror("Error in socket");
        exit(EXIT_FAILURE);
    }

    // Specify server address
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(9002);
    server_address.sin_addr.s_addr = inet_addr("127.0.0.1");

    // Connect to server
    int connection_status = connect(clientB_socket, (struct sockaddr *)&server_address, sizeof(server_address));
    if (connection_status == -1)
    {
        perror("Error in connection");
        exit(EXIT_FAILURE);
    }
    char playAgain = 'Y';

    while (playAgain == 'Y' || playAgain == 'y')
    {
        // Get user input (rock, paper, or scissors)
        char choice;
        printf("Enter your choice (R for Rock, P for Paper, S for Scissors): ");
        scanf(" %c", &choice);

        // Send user choice to server
        int bytes_sent = send(clientB_socket, &choice, sizeof(choice), 0);
        if (bytes_sent == -1)
        {
            perror("Error sending choice to server");
            exit(EXIT_FAILURE);
        }

        // Receive and print the result from the server
        char result;
        int bytes_received = recv(clientB_socket, &result, sizeof(result), 0);
        if (bytes_received == -1)
        {
            perror("Error receiving result from server");
            exit(EXIT_FAILURE);
        }

        if (result == 'D')
        {
            printf("It's a Draw!\n");
        }
        else if (result == 'A')
        {
            printf("Sorry, You Lose! Better luck next time.\n");
        }
        else
        {
            printf("Congratulations! You Win!\n");
        }

        printf("Do you want to play again? (Y/N): ");
        choice = '\0';
        scanf(" %c", &playAgain);

       // Send the playAgain choice to the server
        int playAgainSent = send(clientB_socket, &playAgain, sizeof(playAgain), 0);
        char playAgainReceived;
        int playAgainReceivedBytes = recv(clientB_socket, &playAgainReceived, sizeof(playAgainReceived), 0);

        if (playAgainSent == -1)
        {
            perror("Error sending playAgain choice to server");
            exit(EXIT_FAILURE);
        }
        if(playAgainReceivedBytes == -1)
        {
            perror("Error receiving playAgain choice from server");
            exit(EXIT_FAILURE);
        }
        if(playAgainReceived == 'N')
        {
            printf("The other player has quit the game.\n");
            break;
        }


    } 
    // Close the socket
    close(clientB_socket);

    return 0;
}
