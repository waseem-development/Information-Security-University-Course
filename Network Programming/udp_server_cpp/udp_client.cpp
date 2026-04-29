#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>

#define SERVER_IP "127.0.0.1"
#define UDP_PORT 9090
#define BUF_SIZE 1024

int main()
{
    // STEP 1: Create UDP socket
    int client_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (client_fd < 0)
    {
        std::cerr << "socket error\n";
        return 1;
    }

    // STEP 2: Set up server address
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(UDP_PORT);

    inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr);

    // STEP 3: continuous chat loop
    while (1)
    {
        std::string msg;

        std::cout << "You: ";
        std::getline(std::cin, msg);

        // Send message
        sendto(
            client_fd,
            msg.c_str(),
            msg.length(),
            0,
            (struct sockaddr*)&server_addr,
            sizeof(server_addr)
        );

        if (msg == "exit")
            break;

        // STEP 4: Receive reply
        char buf[BUF_SIZE] = {0};

        struct sockaddr_in reply_addr;
        socklen_t reply_len = sizeof(reply_addr);

        ssize_t n = recvfrom(
            client_fd,
            buf,
            BUF_SIZE - 1,
            0,
            (struct sockaddr*)&reply_addr,
            &reply_len
        );

        if (n > 0)
        {
            buf[n] = '\0';
            std::cout << "Server replied: " << buf << "\n";
        }
    }

    // STEP 5: Close socket
    close(client_fd);
    return 0;
}