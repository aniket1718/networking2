#include <iostream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>

#define SERVER_IP "127.0.0.1"
#define PORT 12345
#define BUF_SIZE 1024

int main() {
    int sock_fd;
    struct sockaddr_in server_addr;
    char buffer[BUF_SIZE];

    // Create socket
    if ((sock_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        std::cerr << "Socket creation failed\n";
        return -1;
    }

    // Set server address
    std::memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    if (inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr) <= 0) {
        std::cerr << "Invalid address or Address not supported\n";
        close(sock_fd);
        return -1;
    }

    // Connect to the server
    if (connect(sock_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        std::cerr << "Connection failed\n";
        close(sock_fd);
        return -1;
    }
    std::cout << "Connected to server\n";

    // Send a message to the server
    const char *message = "Hello from client!";
    send(sock_fd, message, std::strlen(message), 0);

    // Receive the server's response
    int bytes_received = recv(sock_fd, buffer, BUF_SIZE, 0);
    if (bytes_received == -1) {
        std::cerr << "Receive failed\n";
        close(sock_fd);
        return -1;
    }
    buffer[bytes_received] = '\0';  // Null-terminate the received data
    std::cout << "Received from server: " << buffer << "\n";

    // Close the connection
    close(sock_fd);
    return 0;
}
