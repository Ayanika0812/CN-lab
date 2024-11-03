#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 9704
#define BUFFER_SIZE 1024

int main() {
    int sd;
    char buf[BUFFER_SIZE], reply[BUFFER_SIZE];
    struct sockaddr_in sadd, cadd;

    // Create a UDP socket
    sd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sd < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Construct server address
    sadd.sin_family = AF_INET;
    sadd.sin_addr.s_addr = INADDR_ANY; // Bind to any IP
    sadd.sin_port = htons(PORT);

    // Bind the socket to the server address
    if (bind(sd, (struct sockaddr *)&sadd, sizeof(sadd)) < 0) {
        perror("Bind failed");
        close(sd);
        exit(EXIT_FAILURE);
    }

    printf("Server is running and waiting for messages...\n");

    while (1) {
        memset(buf, 0, BUFFER_SIZE);
        socklen_t len = sizeof(cadd);

        // Receive a message from the client
        int m = recvfrom(sd, buf, sizeof(buf), 0, (struct sockaddr *)&cadd, &len);
        if (m < 0) {
            perror("Receive failed");
            continue;
        }

        buf[m] = '\0'; // Null-terminate received message
        printf("Client: %s\n", buf);

        // If the client sends "STOP", terminate the server
        if (strcmp(buf, "STOP") == 0) {
            printf("Stopping the server.\n");
            break;
        }

        // Get a response from the server user
        printf("Enter response to send to client: ");
        fgets(reply, BUFFER_SIZE, stdin);
        reply[strcspn(reply, "\n")] = '\0'; // Remove newline character

        // Send the response back to the client
        sendto(sd, reply, strlen(reply), 0, (struct sockaddr *)&cadd, len);
    }

    close(sd);
    return 0;
}
