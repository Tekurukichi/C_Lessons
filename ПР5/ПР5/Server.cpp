// Server.cpp
#include "Server.h"

Server::Server(int port) : port(port) {
    // Initialize Winsock
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        throw std::runtime_error(GetErrorMessage(WSAGetLastError()));
    }

    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == INVALID_SOCKET) {
        throw std::runtime_error(GetErrorMessage(WSAGetLastError()));
    }

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(port);

    if (bind(serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        throw std::runtime_error(GetErrorMessage(WSAGetLastError()));
    }
}

Server::~Server() {
    for (auto& thread : clientThreads) {
        thread.join();
    }
    closesocket(serverSocket);
    WSACleanup();
}

void Server::Start() {
    if (listen(serverSocket, SOMAXCONN) == SOCKET_ERROR) {
        throw std::runtime_error(GetErrorMessage(WSAGetLastError()));
    }

    while (true) {
        SOCKET clientSocket = accept(serverSocket, NULL, NULL);
        if (clientSocket == INVALID_SOCKET) {
            std::cerr << "accept failed: " << GetErrorMessage(WSAGetLastError()) << std::endl;
            continue;
        }

        clientThreads.push_back(std::thread(&Server::HandleClient, this, clientSocket));
    }
}

void Server::HandleClient(SOCKET clientSocket) {
    try {
        char buffer[1024];
        int bytesReceived = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);

        if (bytesReceived > 0) {
            buffer[bytesReceived] = '\0';
            std::string request(buffer);
            std::string response = ProcessRequest(request);
            if (send(clientSocket, response.c_str(), response.length(), 0) == SOCKET_ERROR) {
                std::cerr << "send failed: " << GetErrorMessage(WSAGetLastError()) << std::endl;
            }
        }
        else if (bytesReceived == 0) {
            std::cout << "Client disconnected." << std::endl;

        }
        else {
            std::cerr << "recv failed: " << GetErrorMessage(WSAGetLastError()) << std::endl;
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Error in HandleClient: " << e.what() << std::endl;
    }
    closesocket(clientSocket);
}

std::string Server::ProcessRequest(const std::string& request) {
    try {
        nlohmann::json jsonRequest = nlohmann::json::parse(request);
        nlohmann::json jsonResponse;
        jsonResponse["message"] = "Response from server: " + jsonRequest.dump();
        return jsonResponse.dump();
    }
    catch (const nlohmann::json::parse_error& e) {
        std::cerr << "Error parsing JSON request: " << e.what() << std::endl;
        return "";
    }
}

std::string Server::GetErrorMessage(int errorCode) {
    LPWSTR messageBuffer = nullptr;
    FormatMessageW(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
        nullptr, errorCode, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPWSTR)&messageBuffer, 0, nullptr);

    int bufferSize = wcslen(messageBuffer) + 1;
    std::wstring wstrErrorMessage(messageBuffer, bufferSize);
    LocalFree(messageBuffer);

    char* charErrorMessage = new char[bufferSize];
    size_t convertedChars = 0;
    wcstombs_s(&convertedChars, charErrorMessage, bufferSize, wstrErrorMessage.c_str(), _TRUNCATE);
    std::string strErrorMessage(charErrorMessage);
    delete[] charErrorMessage;
    return strErrorMessage;
}
