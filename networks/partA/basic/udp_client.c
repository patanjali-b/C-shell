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
    client_socket = socket(AF_INET, SOCK_DGRAM, 0);

    // specify an address for the socket
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(9002);
    server_address.sin_addr.s_addr = inet_addr("127.0.0.1");

    int i;
    i = sendto(client_socket, &client_message, sizeof(client_message), 0, (struct sockaddr *)&server_address, sizeof(server_address));

    if(i < 0)
    {
        perror("Error in sending");
    }
    else if(i == 0)
    {
        printf("No message sent\n");
    }


    int len = sizeof(server_address);
    int j;
    j = recvfrom(client_socket, &server_response, sizeof(server_response), 0, (struct sockaddr *)&server_address, &len);

    if(j < 0)
    {
        perror("Error in receiving");
    }
    else if(j == 0)
    {
        printf("0 bytes received\n");
    }

    printf("The server sent the data: %s\n", server_response);
    

    return 0;
}