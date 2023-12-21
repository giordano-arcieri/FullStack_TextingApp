#include "../include/Client.hpp"
#include <iostream>
#include <stdexcept>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <strings.h>

Client::Client(const std::string& server_ip, int server_port)
{
    //Create socket
    this->client_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(this->client_fd < 0)
    {
        throw std::runtime_error("Client_err: Socket creation failed");
    }

    //The socket API says that you need to zero out the bytes first!
    bzero((char *)&(this->server_address), sizeof(this->server_address));

    //Set server address
    this->server_address.sin_family = AF_INET;
    this->server_address.sin_port = htons(server_port);

    //Convert IPv4 or IPv6 addresses from text to binary form
    if(inet_pton(AF_INET, server_ip.c_str(), &this->server_address.sin_addr) <= 0)
    {
        throw std::runtime_error("Client_err: Invalid address/Address not supported");
    }

    std::cout << "Client set up succesfully!" << std::endl;
}

void Client::connectToServer()
{
    if(connect(client_fd, (struct sockaddr*)&(this->server_address), sizeof(this->server_address)) < 0)
    {
        throw std::runtime_error("Client_err: Connection to server failed");
    }
    std::cout << "Client: Connected to server at " << inet_ntoa(server_address.sin_addr) << ":" << ntohs(server_address.sin_port) << std::endl;
}

void Client::sendMessage(const std::string& message)
{
    send(client_fd, message.c_str(), message.length(), 0);
    std::cout << "Client: Message sent to server: " << message << std::endl;

    // Optionally, you can implement receive logic here if the server sends a response
}

Client::~Client()
{
    close(this->client_fd);
}
