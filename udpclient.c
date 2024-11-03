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
    struct sockaddr_in server_addr;
    char buf[BUFFER_SIZE], reply[BUFFER_SIZE];

    // Create a UDP socket
    sd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sd < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Server address configuration
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1"); // Use localhost
    server_addr.sin_port = htons(PORT);

    printf("Client is ready to send messages. Type 'STOP' to end.\n");

    while (1) {
        // Get message from user
        printf("Enter message to send to server: ");
        fgets(buf, BUFFER_SIZE, stdin);
        buf[strcspn(buf, "\n")] = '\0'; // Remove newline character

        // Send the message to the server
        sendto(sd, buf, strlen(buf), 0, (struct sockaddr *)&server_addr, sizeof(server_addr));

        // If message is "STOP", terminate client
        if (strcmp(buf, "STOP") == 0) {
            printf("Terminating client.\n");
            break;
        }

        // Receive reply from server
        socklen_t len = sizeof(server_addr);
        int n = recvfrom(sd, reply, BUFFER_SIZE, 0, (struct sockaddr *)&server_addr, &len);
        if (n < 0) {
            perror("Receive failed");
            continue;
        }

        reply[n] = '\0'; // Null-terminate the received message
        printf("Server: %s\n", reply);
    }

    close(sd);
    return 0;
}
