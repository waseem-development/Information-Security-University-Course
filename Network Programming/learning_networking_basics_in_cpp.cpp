#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <string>
#include <error.h>
#include <errno.h>

int main()
{
    // Create a socket
    int listening = socket(AF_INET, SOCK_STREAM, 0);
    if (listening <= 0)
    {
        std::cerr << "Can\'t create socket!\n";
        return -1;
    }

    // Bind the socket to an IP / PORT
    sockaddr_in hint;
    hint.sin_family = AF_INET;
    hint.sin_port = htons(54001);
    inet_pton(AF_INET, "0.0.0.0", &hint.sin_addr);

    if (bind(listening, (struct sockaddr *)&hint, sizeof(hint)) == -1)
    {
        std::cerr << "Can\'t bind to IP port!\n";
        std::cerr << "Error: " << strerror(errno) << "\n";
        return -2;
    }

    // Mark the socket for listening in
    if (listen(listening, SOMAXCONN) == -1)
    {
        std::cerr << "Can\'t listen!\n";
        return -3;
    }

    // Accept a call
    sockaddr_in client;
    socklen_t clientSize = sizeof(client);
    char host[NI_MAXHOST];
    char svc[NI_MAXSERV];

    int clientSocket = accept(listening, (sockaddr *)&client, &clientSize);

    if (clientSocket == -1)
    {
        std::cerr << "Problem with client connecting!\n";
        return -4;
    }

    // Close the listening socket
    close(listening);

    memset(host, 0, NI_MAXHOST);
    memset(svc, 0, NI_MAXSERV);

    int result = getnameinfo((struct sockaddr *)&client, sizeof(client), host, NI_MAXHOST, svc, NI_MAXSERV, 0);

    if (result == 0)
    {
        std::cout << host << " connected on " << svc << "\n";
    }
    else
    {
        inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST);
        std::cout << host << " " << ntohs(client.sin_port) << "\n";
    }

    // while receiving, display message, echo message
    char buf[4096];
    while (true)
    {
        // clear the buffer
        memset(buf, 0, 4096);
        // wait for a message
        int byteRecv = recv(clientSocket, buf, 4096, 0);
        // Display Message
        if (byteRecv == -1)
        {
            std::cerr << "There was a connection issue\n";
            break;
        }
        if (byteRecv == 0)
        {
            std::cout << "The Client disconnected\n";
            break;
        }
        std::cout << "Received: " << std::string(buf, 0, byteRecv) << "\n";

        // Resend Message
        send(clientSocket, buf, byteRecv, 0);
    }

    // Close socket
    close(clientSocket);

    return 0;
}