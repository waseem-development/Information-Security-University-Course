#include <iostream>
#include <sys/socket.h> 
#include <netinet/in.h> 
#include <arpa/inet.h>  
#include <unistd.h>     
#include <cstring>      
#include <errno.h>
#include <string>

#define PORT 8080
#define BUF_SIZE 4096 // here it means bytes not bits

int main()
{

    // Step 1: Create socket
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0)
    {
        std::cerr << "Socket creation failed: " << strerror(errno) << "\n";
        return 1;
    }

    // Step 2: setsockopt(): using this we can enable SO_REUSEADDR to prevent 'Address already in use' errors on server restart.
    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)); // Using &opt because setsockopt is designed to recieve a port number

    // Step 3: Bind: Attach the socket to an address and port.
    struct sockaddr_in serverAddress;
    memset(&serverAddress, 0, sizeof(serverAddress));

    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0)
    {
        std::cerr << "bind error!\n";
        return 1;
    }

    // Step 4: Listen: Put the socket in waiting mode to listen to any available devices to set up a queue for them.
    if (listen(server_fd, 10) < 0)
    {
        std::cerr << "Listen Error\n";
        return 1;
    }

    std::cout << "Server listening on port " << PORT << "\n";

    // Step 5: Accept: Block and wait for a client.
    while (1)
    {
        struct sockaddr_in client_addr;
        socklen_t client_len = sizeof(client_addr);

        int client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &client_len);

        if (client_fd < 0)
        {
            std::cerr << "Not accepted!\n";
            continue;
        }

        char client_ip[INET_ADDRSTRLEN];

        // below line converts binary IP to readable text
        inet_ntop(AF_INET,
                  &client_addr.sin_addr,
                  client_ip,
                  sizeof(client_ip));

        std::cout << "Client connected: " << client_ip << "\n";

        // Step 6: recv() Read incoming data
        while (1)
        {
            char buf[BUF_SIZE] = {0};

            ssize_t n = recv(client_fd, buf, BUF_SIZE - 1, 0);

            if (n > 0)
            {
                buf[n] = '\0';
                std::cout << "Client: " << buf << "\n";
            }
            else if (n == 0)
            {
                std::cout << "Client disconnected.\n";
                break;
            }
            else
            {
                std::cerr << "recv() error!\n";
                break;
            }

            if (strcmp(buf, "exit") == 0)
            {
                std::cout << "Chat ended by client.\n";
                break;
            }

            // Step 7: send(): send responses
            std::string reply;

            std::cout << "You: ";
            std::getline(std::cin, reply);

            send(client_fd, reply.c_str(), reply.length(), 0);

            if (reply == "exit")
            {
                std::cout << "Chat ended by server.\n";
                break;
            }
        }

        // STEP 8: Close client socket
        close(client_fd);
    }

    // Step: 9: Close Server socket:
    close(server_fd);

    return 0;
}