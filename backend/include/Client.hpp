#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <netinet/in.h>
#include <string>

class Client
{
private:
    int client_fd; //This will be the file discripter for the socket
    struct sockaddr_in server_address;

public:
    Client(const std::string& server_ip, int server_port);
    void connectToServer();
    void sendMessage(const std::string& message);
    ~Client();
};

#endif