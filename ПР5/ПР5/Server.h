// Server.h
#pragma once

#include <winsock2.h>
#include <ws2tcpip.h>
#include <thread>
#include <vector>
#include <mutex>
#include <json.hpp>
#include <iostream>
#include <string>

#pragma comment(lib, "Ws2_32.lib")

class Server {
public:
    Server(int port);
    ~Server();

    void Start();

private:
    void HandleClient(SOCKET clientSocket);
    std::string ProcessRequest(const std::string& request);
    std::string GetErrorMessage(int errorCode);

    SOCKET serverSocket;
    int port;
    std::vector<std::thread> clientThreads;
    std::mutex dataMutex; // Если нужны разделяемые ресурсы
};