#include <iostream>
#include "../include/Server.hpp"
#include "../include/Client.hpp"
#include <unistd.h>

int main()
{
    //start server
    //make two clients
    pid_t pid = fork();
    if(pid == 0)
    {
        //This is the child process
        Server s {8003};
        s.run();
    } 

    sleep(5);


    Client c {"127.0.0.1", 8003};
    c.connectToServer();


    return 0;
}