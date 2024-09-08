// TCP Concurrent Server to remove duplicate words
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <ctype.h>

#define PORTNO 10200

// Function to remove duplicate words from a sentence
void remove_duplicates(char *str, char *result) {
    char *words[100];
    int word_count = 0;
    char *token = strtok(str, " ");
    
    // Split sentence into words
    while (token != NULL) {
        words[word_count++] = token;
        token = strtok(NULL, " ");
    }

    // Check for duplicates and form the result
    int i, j, k, duplicate;
    for (i = 0; i < word_count; i++) {
        duplicate = 0;
        for (j = 0; j < i; j++) {
            if (strcmp(words[i], words[j]) == 0) {
                duplicate = 1; // Found a duplicate
                break;
            }
        }
        if (!duplicate) {
            strcat(result, words[i]);
            strcat(result, " ");
        }
    }
    result[strlen(result) - 1] = '\0'; // Remove the trailing space
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

            char buffer[1024] = {0}, result[1024] = {0};
            int n;

            // Read the sentence from the client
            n = read(newsockfd, buffer, sizeof(buffer));
            if (n < 0) {
                perror("Error reading from client");
                close(newsockfd);
                exit(1);
            }
            buffer[n] = '\0'; // Null-terminate the received string
            printf("Received sentence from client: %s\n", buffer);

            // Process the sentence to remove duplicates
            remove_duplicates(buffer, result);

            // Send the resultant sentence back to the client
            n = write(newsockfd, result, strlen(result));
            if (n < 0) {
                perror("Error writing to client");
            }

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
Received sentence from client: hi
Received sentence from client: heeloo from c2
Received sentence from client: hi hi hi from c1
*/