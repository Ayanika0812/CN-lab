// TCP Client Program to send sentence and receive result after duplicate removal
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORTNO 10200

int main() {
    int sockfd;
    struct sockaddr_in address;
    char buffer[1024];

    // Create the socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Error opening socket");
        exit(1);
    }

    // Setup server address struct
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr("127.0.0.1"); // Server IP address (localhost)
    address.sin_port = htons(PORTNO);

    // Connect to the server
    if (connect(sockfd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Error connecting to server");
        close(sockfd);
        exit(1);
    }

    // Get sentence from user
    printf("Enter a sentence: ");
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = '\0'; // Remove the trailing newline

    // Send the sentence to the server
    write(sockfd, buffer, strlen(buffer));

    // Receive the result from the server
    int n = read(sockfd, buffer, sizeof(buffer));
    if (n < 0) {
        perror("Error reading from server");
        close(sockfd);
        exit(1);
    }
    buffer[n] = '\0'; // Null-terminate the received string

    // Display the result
    printf("Modified sentence: %s\n", buffer);

    // Close the connection
    close(sockfd);

    return 0;
}

/*

/c1
Enter a sentence: hi hi hi from c1
Modified sentence: hi from c1

./c2
Enter a sentence: heeloo from c2
Modified sentence: heeloo from c2

*/