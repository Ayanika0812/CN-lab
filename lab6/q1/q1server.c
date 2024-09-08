// TCP Math Server Program (Concurrent)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORTNO 10200

int perform_operation(int num1, int num2, char operator) {
    switch(operator) {
        case '+': return num1 + num2;
        case '-': return num1 - num2;
        case '*': return num1 * num2;
        case '/': return (num2 != 0) ? num1 / num2 : 0; // Check for division by zero
        default: return 0;
    }
}

int main() {
    int sockfd, newsockfd, clilen;
    struct sockaddr_in seraddr, cliaddr;

    // Create the socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Error opening socket");
        exit(1);
    }

    // Setup server address struct
    seraddr.sin_family = AF_INET;
    seraddr.sin_addr.s_addr = inet_addr("127.0.0.1"); // Server IP address (localhost)
    seraddr.sin_port = htons(PORTNO);

    // Bind the socket to the address
    if (bind(sockfd, (struct sockaddr *)&seraddr, sizeof(seraddr)) < 0) {
        perror("Error binding socket");
        close(sockfd);
        exit(1);
    }

    // Listen for incoming connections
    listen(sockfd, 5);
    printf("Server listening on port %d...\n", PORTNO);

    while (1) {
        clilen = sizeof(cliaddr);
        // Accept a new client connection
        newsockfd = accept(sockfd, (struct sockaddr *)&cliaddr, &clilen);
        if (newsockfd < 0) {
            perror("Error accepting connection");
            continue;
        }

        // Fork a new process to handle the client
        if (fork() == 0) {
            // Child process: handle client communication
            close(sockfd);

            int num1, num2, result;
            char operator;

            // Read two integers and operator from client
            read(newsockfd, &num1, sizeof(num1));
            read(newsockfd, &num2, sizeof(num2));
            read(newsockfd, &operator, sizeof(operator));

            printf("Received operation: %d %c %d\n", num1, operator, num2);

            // Perform the arithmetic operation
            result = perform_operation(num1, num2, operator);

            // Send the result back to the client
            write(newsockfd, &result, sizeof(result));

            // Close the connection
            close(newsockfd);
            exit(0);
        } else {
            // Parent process: close the client's socket and continue listening
            close(newsockfd);
        }
    }

    return 0;
}


/*

gcc -o s q1server.c
 ls
c1  c2  q1client.c  q1server.c  s
 ./s
Server listening on port 10200...
Received operation: 2 + 3
Received operation: 3 * 4
Received operation: 3 * 5


*/