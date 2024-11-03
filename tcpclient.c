#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    int sockfd;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE];
    int bytes_received;

    // Creating socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Setting server details
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");  // Direct IP assignment

    // Connecting to the server
    if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Connection failed");
        exit(EXIT_FAILURE);
    }

    printf("Connected to the server.\n");

    // Sending messages to the server and receiving responses
    while (1) {
        printf("Enter message (or STOP to quit): ");
        fgets(buffer, BUFFER_SIZE, stdin);
        buffer[strcspn(buffer, "\n")] = '\0'; // Remove newline character
        
        // Send message to the server
        send(sockfd, buffer, strlen(buffer), 0);
        
        // If message is "STOP", terminate client
        if (strcmp(buffer, "STOP") == 0) {
            printf("Terminating client.\n");
            break;
        }

        // Receive response from the server
        bytes_received = recv(sockfd, buffer, BUFFER_SIZE, 0);
        if (bytes_received > 0) {
            buffer[bytes_received] = '\0'; // Null-terminate the received data
            printf("Server response: %s\n", buffer);
        } else if (bytes_received == 0) {
            printf("Server closed the connection.\n");
            break;
        } else {
            perror("Receive failed");
            break;
        }
    }

    // Close the socket connection
    close(sockfd);
    return 0;
}
