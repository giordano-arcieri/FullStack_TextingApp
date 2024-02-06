#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>

#define MAX_CLIENTS 30
#define BUFFER_SIZE 1024
#define PORT 8080


void handleClient(int clientSocket)
{
    char buffer[BUFFER_SIZE];
    std::cout << "Client connected" << std::endl;

    // Read data from client
    read(clientSocket, buffer, BUFFER_SIZE);
    std::cout << "Client says: " << buffer << std::endl;

    // Close client socket
    close(clientSocket);
}

int main()
{
    int server_fd, clientSocket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);

    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Attaching socket to the port 8080
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, MAX_CLIENTS) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    while (true)
    {
        if ((clientSocket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0)
        {
            perror("accept");
            continue;
        }

        int pid = fork();
        if (pid < 0)
        {
            perror("fork failed");
            close(clientSocket);
            continue;
        }

        if (pid == 0)
        {                     // This is the child process
            close(server_fd); // Child does not need the listener
            handleClient(clientSocket);
            exit(0);
        }
        else
        {                        // Parent process
            close(clientSocket); // Parent does not need this
        }
    }

    close(server_fd);
    return 0;
}
