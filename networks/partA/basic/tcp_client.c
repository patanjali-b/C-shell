#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main()
{
    int client_socket;
    struct sockaddr_in server_address;
    char server_response[256];
    char client_message[256] = "Hello from client";



    // create socket
    client_socket = socket(AF_INET, SOCK_STREAM, 0);

    // specify an address for the socket
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(9002);
    server_address.sin_addr.s_addr = inet_addr("127.0.0.1");

    int connection_status = connect(client_socket, (struct sockaddr *)&server_address, sizeof(server_address));

    // check for error with the connection
    if (connection_status == -1)
    {
        printf("There was an error making a connection to the remote socket \n\n");
    }

    // receive data from the server
    int i;
    i = recv(client_socket, &server_response, sizeof(server_response), 0);
    if (i < 0)
    {
        perror("Error in receiving");
    }
    else if (i == 0)
    {
        printf("0 bytes sent\n");
    }

    int j;
    j = send(client_socket, &client_message, sizeof(client_message), 0);
    if (j < 0)
    {
        perror("Error in sending by client");
    }
    else if (j == 0)
    {
        printf("No message sent by the client\n");
    }

    // print out the server's response
    printf("The server sent the data: %s\n", server_response);

    // close the socket
    close(client_socket);

    return 0;
}