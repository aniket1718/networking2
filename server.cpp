#include <iostream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 12345
#define BUF_SIZE 1024

int main() {
    int server_fd, client_fd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_addr_len = sizeof(client_addr);
    char buffer[BUF_SIZE];

    // Create server socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        std::cerr << "Socket creation failed\n";
        return -1;
    }

    // Set server address
    std::memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    // Bind the socket to address and port
    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        std::cerr << "Bind failed\n";
        close(server_fd);
        return -1;
    }

    // Listen for incoming connections
    if (listen(server_fd, 3) == -1) {
        std::cerr << "Listen failed\n";
        close(server_fd);
        return -1;
    }
    std::cout << "Server is listening on port " << PORT << "...\n";

    // Accept a connection from the client
    if ((client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &client_addr_len)) == -1) {
        std::cerr << "Accept failed\n";
        close(server_fd);
        return -1;
    }
    std::cout << "Connection established with client\n";

    // Receive a message from the client
    int bytes_received = recv(client_fd, buffer, BUF_SIZE, 0);
    if (bytes_received == -1) {
        std::cerr << "Receive failed\n";
        close(client_fd);
        close(server_fd);
        return -1;
    }
    buffer[bytes_received] = '\0';  // Null-terminate the received data
    std::cout << "Received from client: " << buffer << "\n";

    // Send a response to the client
    const char *message = "Hello from server!";
    send(client_fd, message, std::strlen(message), 0);

    // Close the connection
    close(client_fd);
    close(server_fd);
    return 0;
}
