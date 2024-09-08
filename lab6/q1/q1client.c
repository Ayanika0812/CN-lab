// TCP Math Client Program
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORTNO 10200

int main() {
    int sockfd;
    struct sockaddr_in address;
    int num1, num2, result;
    char operator;

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

    // Get two integers and operator from user
    printf("Enter first integer: ");
    scanf("%d", &num1);
    printf("Enter second integer: ");
    scanf("%d", &num2);
    printf("Enter operator (+, -, *, /): ");
    scanf(" %c", &operator);

    // Send the integers and operator to the server
    write(sockfd, &num1, sizeof(num1));
    write(sockfd, &num2, sizeof(num2));
    write(sockfd, &operator, sizeof(operator));

    // Receive the result from the server
    read(sockfd, &result, sizeof(result));

    // Display the result
    printf("Result: %d\n", result);

    // Close the connection
    close(sockfd);

    return 0;
}


/*


gcc -o c2 q1client.c
 ./c1
Enter first integer: 2
Enter second integer: 3
Enter operator (+, -, *, /): +
Result: 5
./c1
Enter first integer: 3
Enter second integer: 5
Enter operator (+, -, *, /): *
Result: 15


./c2
Enter first integer: 3
Enter second integer: 4
Enter operator (+, -, *, /): *
Result: 12

*/