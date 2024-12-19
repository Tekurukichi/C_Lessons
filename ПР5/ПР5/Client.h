//Client.h
#pragma once

#include <winsock2.h>
#include <ws2tcpip.h>
#include <string>
#include <json.hpp>
#include <iostream>


class Client {
public:
    Client(const std::string& serverAddress, int port);
    ~Client();

    void Connect();
    std::string SendRequest(const std::string& request);
    std::string GetErrorMessage(int errorCode);

private:
    SOCKET clientSocket;
    std::string serverAddress;
    int port;
};
