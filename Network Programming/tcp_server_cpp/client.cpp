#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>
#include <cstring>
#include <string>

#define PORT 8080
#define BUF_SIZE 4096

int main()
{

    // Step 1: Create socket and return an integer file descriptor
    int client_socket = socket(AF_INET, SOCK_STREAM, 0);

    if (client_socket < 0)
    {
        std::cerr << "Socket creation failed: " << strerror(errno) << "\n";
        return 1;
    }

    // Step 2: Connect: Perform the TCP three-way handshake with the server
    struct sockaddr_in serverAddress;

    memset(&serverAddress, 0, sizeof(serverAddress));

    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(PORT);
    
    // In the following line, we use inet_pton to convert our ip address (127.0.0.1) to machine readable binary format so the machine can understand it
    if (inet_pton(AF_INET, "127.0.0.1", &serverAddress.sin_addr) <= 0)
    {
        std::cerr << "inet_pton() failed\n";
        return 1;
    }

    if (connect(
            client_socket,
            (struct sockaddr *)&serverAddress,
            sizeof(serverAddress)) < 0)
    {
        std::cerr << "Failed to establish connection\n";
        return 1;
    }

    std::cout << "Connection to server established\n";

    while (1)
    {
        // Step: 3: Send: Write data into the socket's send buffer
        std::string message;

        std::cout << "You: ";
        std::getline(std::cin, message);

        ssize_t bytes_sent = send(client_socket, message.c_str(), message.length(), 0);

        if (bytes_sent < 0)
        {
            std::cerr << "send() failed\n";
            break;
        }

        if (message == "exit")
        {
            break;
        }

        // Step 4: recv: Receive the server's response. Blocks
        char buf[BUF_SIZE] = {0};

        ssize_t n = recv(client_socket, buf, BUF_SIZE - 1, 0);

        if (n > 0)
        {
            buf[n] = '\0';
            std::cout << "Server replied: " << buf << "\n";
        }
        else if (n == 0)
        {
            std::cout << "Server closed the connection\n";
            break;
        }
        else
        {
            std::cerr << "recv() error!\n";
            break;
        }

        if (strcmp(buf, "exit") == 0)
        {
            break;
        }
    }

    // Step 5: Close: Terminate the connection and free OS resources.
    close(client_socket);

    return 0;
}