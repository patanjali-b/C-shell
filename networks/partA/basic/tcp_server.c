#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main()
{
    int server_socket;
    struct sockaddr_in server_address;
    char server_message[256] = "You have reached the server!";

    // create the server socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);

    // define the server address
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(9002);
    server_address.sin_addr.s_addr = inet_addr("127.0.0.1");

    // bind the socket to our specified IP and port
    int k =bind(server_socket, (struct sockaddr *)&server_address, sizeof(server_address));
    if(k < 0)
    {
        perror("Error in binding");
    }
    listen(server_socket, 5);

    struct sockaddr_in client_address;

    int client_socket; 
    int addrlen = sizeof(client_address);
    client_socket = accept(server_socket, (struct sockaddr *)&client_address, &addrlen);

    if(client_socket < 0)
    {
        perror("Error in accepting");
    }

    int i; 
    i = send(client_socket, &server_message, sizeof(server_message), 0);
    if(i < 0)
    {
        perror("Error in sending");
    }
    else if(i == 0)
    {
        printf("No message sent\n");
    }

    char client_response[256];
    int j;
    j = recv(client_socket, &client_response, sizeof(client_response), 0);
    printf("The client sent the data: %s\n", client_response);

    if(j < 0)
    {
        perror("Error in receiving");
    }
    else if(j == 0)
    {
        printf("0 bytes received\n");
    }

    return 0;    
}