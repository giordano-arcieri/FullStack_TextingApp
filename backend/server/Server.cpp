#include "../include/Server.hpp"
#include <unistd.h>
#include <strings.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <iostream>

Server::Server(int port)
{
    //The constructor will set up all necessary components of the network conenction on the server side
    //Creating a socket and a file descriptor for the connection
    this->server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(this->server_fd < 0)
    {
        throw std::runtime_error("Server_err: Socket creation failed");
    }

    //Set socket options 
    int opt = 1;
    if(setsockopt(this->server_fd, SOL_SOCKET, SO_REUSEADDR, (const void*)&opt, sizeof(opt)) < 0)
    {
        throw std::runtime_error("Server_err: Setting socket options failed");
    }
    
    //The socket API says that you need to zero out the bytes first!
    bzero((char*)&(this->address), sizeof(this->address));

    this->address.sin_family = AF_INET; //This will set up AF_INET as protocol family
    this->address.sin_addr.s_addr = htonl(INADDR_ANY); //This means that it will allow all IP adresses 
    this->address.sin_port = htons((unsigned short)port); //This is the port number the server will listen too

    //This will binf the servers information with the socket fd
    if(bind(this->server_fd, (struct sockaddr*)&(this->address), sizeof(this->address)) < 0)
    {
        throw std::runtime_error("Server_err: Bind failed");
    }

    std::cout << "Server succesfully initialized at port: "<< htons(this->address.sin_port) << std::endl;
}

int Server::run()
{
    int addrlen = sizeof(this->address);

    //Start listening for incoming connections
    if(listen(this->server_fd, MAX_NUMBER_OF_CLIENTS) < 0)
    {
        //Here if we fail we both throw an exeprion and return -1. I am not sure if this is raccomended.
        throw std::runtime_error("Server_err: Listen failed");
        return -1;
    }

    std::cout << "Server: Waiting for connections..." << std::endl; //This is mostly for dubugging and making sure everything works right. Obviously no print statments are really needed

    while(true)
    {
        //This next bit waits for a client to try to connect and creates a new process to handle that client
        int new_client_fd = accept(server_fd, (struct sockaddr*)&(this->address), (socklen_t*)&addrlen);
        if(new_client_fd < 0)
        {
            std::cerr << "Server_err: error making a new socket" << std::endl;
            continue; //This is needed as we dont want to make a new process if no client was made so we just continue to next loop
        }

        std::cout << "Server: Client Connected!" << std::endl;

        //Now we make a new process to take care of that client. We could have used either threads or process. 
        //I decided to use process as they are safer and easier. 
        pid_t pid = fork();
        if(pid == 0)
        {
            //This is the child process
            //Since we are in the child process we only need to worry about our client and we do not need the server_fd
            close(this->server_fd); 
            //Now we simply hadnle the client
            handle_client(new_client_fd);
        } 
        else if(pid > 0)
        {
            //This is the parent process
            close(new_client_fd); //Close the client socket in the parent process
        }
        else
        {
            //Fork failed
            std::cerr << "Server_err: Fork failed" << std::endl;
            close(new_client_fd);
            continue;
        }
    }

    //This will never happen since we have a while true. I intend to change this so it only runs for a certain amount of seconds.
    return 0;
}

void Server::handle_client(int client_fd)
{
    std::cout << "Server: Handling client ...\n" << std::endl;

    
    sleep(10);


    close(client_fd); //Close the client socket
    exit(0); //Exit the child process
}

Server::~Server()
{
    //We need to just close the fd nothing else has to be freed
    close(this->server_fd);
}

