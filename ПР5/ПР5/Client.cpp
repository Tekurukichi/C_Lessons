// Client.cpp
#include "Client.h"
#include "Server.h"


Client::Client(const std::string& serverAddress, int port) : serverAddress(serverAddress), port(port) {
    // Initialize Winsock
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "WSAStartup failed: " << GetErrorMessage(WSAGetLastError()) << std::endl;
        throw std::runtime_error("WSAStartup failed");
    }

    // Create a socket
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == INVALID_SOCKET) {
        std::cerr << "socket failed: " << GetErrorMessage(WSAGetLastError()) << std::endl;
        throw std::runtime_error("socket failed");
    }
}

Client::~Client() {
    closesocket(clientSocket);
    WSACleanup();
}

void Client::Connect() {
    // Connect to server
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;


    inet_pton(AF_INET, serverAddress.c_str(), &serverAddr.sin_addr);
    serverAddr.sin_port = htons(port);
    if (connect(clientSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "connect failed: " << GetErrorMessage(WSAGetLastError()) << std::endl;
        throw std::runtime_error("connect failed");
    }
}

std::string Client::SendRequest(const std::string& request) {

    if (send(clientSocket, request.c_str(), request.length(), 0) == SOCKET_ERROR) {
        std::cerr << "send failed: " << GetErrorMessage(WSAGetLastError()) << std::endl;
        throw std::runtime_error("send failed");
    }

    char buffer[1024];
    int bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);
    if (bytesReceived > 0) {
        buffer[bytesReceived] = '\0';
        return std::string(buffer);
    }
    else if (bytesReceived == 0) {
        std::cerr << "Server disconnected." << std::endl;
        return "";
    }
    else {
        std::cerr << "recv failed: " << GetErrorMessage(WSAGetLastError()) << std::endl;
        throw std::runtime_error("recv failed");
    }
}
std::string Client::GetErrorMessage(int errorCode) {
    //  -- та же реализация, что и в Server::GetErrorMessage -- //
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



int main() {
    try {
        Server server(12345);
        std::thread serverThread([&server]() { server.Start(); });

        Client client("127.0.0.1", 12345);
        client.Connect();

        // Example JSON request
        nlohmann::json jsonRequest;
        jsonRequest["command"] = "get_data";
        jsonRequest["parameter"] = 123;
        std::string request = jsonRequest.dump();

        std::string response = client.SendRequest(request);


        if (!response.empty()) {
            std::cout << "Server Response: " << response << std::endl;

            //Parse the response
            try {
                nlohmann::json jsonResponse = nlohmann::json::parse(response);
                std::cout << "Parsed response: " << jsonResponse["message"] << std::endl;
            }
            catch (const nlohmann::json::parse_error& e) {
                std::cerr << "Error parsing JSON response: " << e.what() << std::endl;
            }


        }


        serverThread.detach();
        system("pause");

    }
    catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
        system("pause");
        return 1;
    }

    return 0;
}