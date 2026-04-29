#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>

#define UDP_PORT 9090
#define BUF_SIZE 1024

int main() {
    // Step 1: Create UPD Socket
    int server_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (server_fd < 0) {
        std::cerr << "Socket Creation Error\n";
        return -1;
    }

    // Step 2: Change Socket Options
    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    // Step 3: Bind
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(UDP_PORT);

    if (bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        std::cerr << "Bind error\n";
        return -1;
    }

    // Step 4: Recieve and send
    while (1) {
        char buf[BUF_SIZE] = {0};
        struct sockaddr_in client_addr;
        socklen_t client_len = sizeof(client_addr);

        ssize_t n = recvfrom(
            server_fd,
            buf,
            BUF_SIZE - 1,
            0,
            (struct sockaddr*)&client_addr,
            &client_len
        );

        if (n < 0) {
            std::cerr << "Recieve error\n";
            continue;
        }

        buf[n] = '\0';

        char client_ip[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &client_addr.sin_addr, client_ip, sizeof(client_ip));

        std::cout << "Datagram from " << client_ip << " " << buf << "\n";

        // Send reply to the sender
        const char *reply = "UDP reply from server!";
        sendto(server_fd, reply, strlen(reply), 0,
               (struct sockaddr*)&client_addr, client_len);
    }

    close(server_fd);
    return 0;
}