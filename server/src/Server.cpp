#include <pistache/endpoint.h>
#include <pistache/router.h>
#include <nlohmann/json.hpp>
#include <fstream>
#include <string>
#include <vector>
#include <iostream>
#include <mutex>

using namespace Pistache;
using json = nlohmann::json;

#define DATA_FOLDER "/home/giordano-arcieri/Gio/Projects/inProgress/network_programming/server/data/";
#define ONLINE_USERES_FILE_NAME "online_users.json";
#define PORT 9080

class Handler
{
public:
    // Need to call constructor then init and run for the server to become online.
    
    explicit Handler(Address address) : httpEndpoint(address) { std::cout << "[SERVER] Server starting..." << std::endl; };

    void init(const Http::Endpoint::Options &options)
    {
        std::cout << "[SERVER] Server initializing..." << std::endl;
        httpEndpoint.init(options);
        setupRoutes();
    }

    void run()
    {
        httpEndpoint.setHandler(router.handler());
        std::cout << "[SERVER] Server is running." << std::endl;
        httpEndpoint.serve();
    }

    void shutdown()
    {
        std::cout << "[SERVER] Server is shutting down..." << std::endl;

        // Get file paths for database
        std::string online_users_file_path = DATA_FOLDER;
        online_users_file_path += ONLINE_USERES_FILE_NAME;
        std::string messages_directory_path = DATA_FOLDER;
        messages_directory_path += "messages/";

        // Delete database
        if (std::filesystem::exists(messages_directory_path) && std::filesystem::is_directory(messages_directory_path))
        {
            // Iterate through the directory
            for (const auto &entry : std::filesystem::directory_iterator(messages_directory_path))
            {
                if (std::filesystem::is_regular_file(entry.path()))
                {
                    std::filesystem::remove(entry.path());
                }
            }
        }
        if (std::filesystem::exists(online_users_file_path))
        {
            std::filesystem::remove(online_users_file_path);
        }

        // Shutdown server
        httpEndpoint.shutdown();

        std::cout << "[SERVER] Server shut down." << std::endl;
    }

private:
    void setupRoutes()
    {
        using namespace Rest;

        Routes::Get(router, "/get_online_users", Routes::bind(&Handler::handleGetOnlineUseres, this));
        Routes::Get(router, "/getMessages", Routes::bind(&Handler::handleGetMessages, this));
        Routes::Post(router, "/newLogin", Routes::bind(&Handler::handleNewLogin, this));
        Routes::Post(router, "/sendMessage", Routes::bind(&Handler::handleSendMessage, this));
        Routes::Delete(router, "/LogOff", Routes::bind(&Handler::handleLogOff, this));
        Routes::Post(router, "/shutdown", Routes::bind(&Handler::handleSutdownRequest, this));
    }

    void handleGetOnlineUseres(const Rest::Request &request, Http::ResponseWriter response)
    {
        std::lock_guard<std::mutex> guard(mutex_);

        // Get file path
        std::string online_useres_file_path = DATA_FOLDER;
        online_useres_file_path += ONLINE_USERES_FILE_NAME;

        // Open file
        std::ifstream file_online_users(online_useres_file_path);
        if (!file_online_users.is_open())
        {
            std::cerr << "[ERROR] Unable to open database file." << std::endl;
            response.send(Http::Code::Internal_Server_Error, "Internal Server Error\n");
            return;
        }

        // Upload data to JSON
        json json_online_users;
        file_online_users >> json_online_users;
        file_online_users.close();

        // Send JSON
        std::cout << "[INFO] GET request received sending back JSON of online users:" << json_online_users.dump() << std::endl;
        response.send(Http::Code::Ok, json_online_users.dump());
    }

    void handleGetMessages(const Rest::Request &request, Http::ResponseWriter response)
    {
        std::lock_guard<std::mutex> guard(mutex_);

        // Get file path of messages
        json user_info = json::parse(request.body());
        std::string messages_file_path = DATA_FOLDER;
        messages_file_path += "messages/";
        messages_file_path += user_info["username"];
        messages_file_path += ".json";

        // Check if file exists
        if (!std::filesystem::exists(messages_file_path))
        {
            std::cerr << "[ERROR] Unable to open database file." << std::endl;
            response.send(Http::Code::Bad_Request, "Non existing user\n");
            return;
        }

        // Open file
        std::ifstream file_messages(messages_file_path);
        if (!file_messages.is_open())
        {
            std::cerr << "[ERROR] Unable to open database file." << std::endl;
            response.send(Http::Code::Internal_Server_Error, "Internal Server Error\n");
            return;
        }

        // Upload data to JSON
        json json_messages;
        file_messages >> json_messages;
        file_messages.close();

        // Send JSON
        std::cout << "[INFO] GET request received sending back JSON of messages:" << json_messages.dump() << std::endl;
        response.send(Http::Code::Ok, json_messages.dump());
    }

    void handleNewLogin(const Rest::Request &request, Http::ResponseWriter response)
    {
        std::lock_guard<std::mutex> guard(mutex_);

        // Updload request to JSON
        json new_user_info = json::parse(request.body());

        // Get file path for online users
        std::string online_useres_file_path = DATA_FOLDER;
        online_useres_file_path += ONLINE_USERES_FILE_NAME;

        // Set up file
        std::ifstream iFile_online_users(online_useres_file_path);
        if (!iFile_online_users.is_open())
        {
            std::cerr << "[ERROR] Unable to open database file." << std::endl;
            response.send(Http::Code::Internal_Server_Error, "Internal Server Error\n");
            return;
        }

        // Upload data to JSON
        json json_online_users;
        iFile_online_users >> json_online_users;
        iFile_online_users.close();

        // Check if user already exists
        for (std::string &user : json_online_users["online_users"].get<std::vector<std::string>>())
        {
            if (user == new_user_info["username"])
            {
                std::cerr << "[ERROR] User already exists." << std::endl;
                response.send(Http::Code::Conflict, "User already exists\n");
                return;
            }
        }

        // Add user to JSON
        json_online_users["online_users"].push_back(new_user_info["username"]);

        // Save JSON to file
        std::ofstream oFile_online_users(online_useres_file_path);
        if (!oFile_online_users.is_open())
        {
            std::cerr << "[ERROR] Unable to open database file." << std::endl;
            response.send(Http::Code::Internal_Server_Error, "Internal Server Error\n");
            return;
        }
        oFile_online_users << json_online_users.dump(4);
        oFile_online_users.close();

        // Add Message file for user
        std::string messages_file_path = DATA_FOLDER;
        messages_file_path += "messages/";
        messages_file_path += new_user_info["username"];
        messages_file_path += ".json";
        std::ofstream file_messages(messages_file_path);
        if (!file_messages.is_open())
        {
            std::cerr << "[ERROR] Unable to create user message file for new user." << std::endl;
            response.send(Http::Code::Internal_Server_Error, "Internal Server Error\n");
            return;
        }
        json j;
        j["messages"] = json::array();
        file_messages << j.dump(4);
        file_messages.close();

        // Send response
        std::cout << "[INFO] Added new user: " << new_user_info["username"] << std::endl;
        response.send(Http::Code::Created, "NewLogin Recived!\n");
    }

    void handleSendMessage(const Rest::Request &request, Http::ResponseWriter response)
    {
        std::lock_guard<std::mutex> guard(mutex_);

        // Updload request to JSON
        json new_message_info = json::parse(request.body());

        // Get file path for messages
        std::string messages_file_path = DATA_FOLDER;
        messages_file_path += "messages/";
        messages_file_path += new_message_info["reciver"];
        messages_file_path += ".json";

        // Check if file exists
        if (!std::filesystem::exists(messages_file_path))
        {
            std::cerr << "[ERROR] Unable to open database file." << std::endl;
            response.send(Http::Code::Bad_Request, "Tried to send message to non existing user\n");
            return;
        }

        // Open file
        std::ifstream iFile_messages(messages_file_path);
        if (!iFile_messages.is_open())
        {
            std::cerr << "[ERROR] Unable to open database file." << std::endl;
            response.send(Http::Code::Internal_Server_Error, "Internal Server Error\n");
            return;
        }

        // Upload data to JSON
        json json_user_messages;
        iFile_messages >> json_user_messages;
        iFile_messages.close();

        // Add message to JSON
        json new_message;
        new_message["sender"] = new_message_info["sender"];
        new_message["content"] = new_message_info["message"];
        json_user_messages["messages"].push_back(new_message);

        // Save JSON to file
        std::ofstream oFile_messages(messages_file_path);
        if (!oFile_messages.is_open())
        {
            std::cerr << "[ERROR] Unable to open database file." << std::endl;
            response.send(Http::Code::Internal_Server_Error, "Internal Server Error\n");
            return;
        }
        oFile_messages << json_user_messages.dump(4);
        oFile_messages.close();

        // Send response
        std::cout << "[INFO] Added Message: [ " << new_message_info["message"] << " ] to " << new_message_info["reciver"] << std::endl;
        response.send(Http::Code::Created, "Message Recived!\n");
    }

    void handleLogOff(const Rest::Request &request, Http::ResponseWriter response)
    {
        std::lock_guard<std::mutex> guard(mutex_);

        // Updload request to JSON
        json log_off_info = json::parse(request.body());

        // Get file path for online users
        std::string online_useres_file_path = DATA_FOLDER;
        online_useres_file_path += ONLINE_USERES_FILE_NAME;

        // Updload data to JSON
        std::ifstream iFile_online_users(online_useres_file_path);
        if (!iFile_online_users.is_open())
        {
            std::cerr << "[ERROR] Unable to open database file." << std::endl;
            response.send(Http::Code::Internal_Server_Error, "Internal Server Error\n");
            return;
        }
        json json_online_users;
        iFile_online_users >> json_online_users;
        iFile_online_users.close();

        // Check if user exists
        bool user_exists = false;
        for (std::string &user : json_online_users["online_users"].get<std::vector<std::string>>())
        {
            if (user == log_off_info["username"])
            {
                json_online_users["online_users"].erase(std::remove(json_online_users["online_users"].begin(), json_online_users["online_users"].end(), log_off_info["username"]), json_online_users["online_users"].end());
                user_exists = true;
                break;
            }
        }
        if (!user_exists)
        {
            std::cerr << "[ERROR] User does not exist." << std::endl;
            response.send(Http::Code::Bad_Request, "User does not exist\n");
            return;
        }

        // Save JSON to file
        std::ofstream oFile_online_users(online_useres_file_path);
        if (!oFile_online_users.is_open())
        {
            std::cerr << "[ERROR] Unable to open database file." << std::endl;
            response.send(Http::Code::Internal_Server_Error, "Internal Server Error\n");
            return;
        }
        oFile_online_users << json_online_users.dump(4);
        oFile_online_users.close();

        // Delete message file for user
        std::string messages_file_path = DATA_FOLDER;
        messages_file_path += "messages/";
        messages_file_path += log_off_info["username"];
        messages_file_path += ".json";
        if (std::filesystem::exists(messages_file_path))
        {
            std::filesystem::remove(messages_file_path);
        }

        // Send response
        std::cout << "[INFO] Deleted " << log_off_info["username"] << std::endl;
        response.send(Http::Code::Ok, "User Deleted!\n");
    }

    void handleSutdownRequest(const Rest::Request &request, Http::ResponseWriter response)
    {
        std::lock_guard<std::mutex> guard(mutex_);
        std::cout << "[INFO] Shutdown request received. Shutting down server..." << std::endl;
        response.send(Http::Code::Ok, "Server is shutting down...\n");
        std::thread([this]()
                    { this->shutdown(); })
            .detach();
    }

    Http::Endpoint httpEndpoint;
    Rest::Router router;

    std::mutex mutex_;
};

int main()
{
    // Create Database
    std::string data = DATA_FOLDER;
    data += ONLINE_USERES_FILE_NAME;
    std::cout << data << std::endl;
    std::ofstream file_online_users(data);

    if (!file_online_users.is_open())
    {
        std::cerr << "[ERROR] Unable to create database file." << std::endl;
        return 1;
    }

    json j;
    j["online_users"] = json::array();
    file_online_users << j.dump(4);
    file_online_users.close();

    // Start Server
    Http::Endpoint::Options options = Http::Endpoint::options().threads(1);
    Address address(Ipv4::any(), Port(PORT));

    Handler handler(address);

    handler.init(options);
    handler.run();

    return 0;
}