#ifndef SERVER_HPP
#define SERVER_HPP
#include <netinet/in.h>


#define MAX_NUMBER_OF_CLIENTS 100

class Server
{
private:
    int server_fd; //This will be the file discripter for the socket
    struct sockaddr_in address; 

public:
    Server(int port);
    int run(void);
    void handle_client(int client_fd);
    ~Server();
};

#endif