#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <vector>
#include <thread>
#include <mutex>
#include <algorithm>

std::vector<int> clients;
std::mutex clientsMutex;

void broadcast(const std::string& msg, int senderSocket) {
    std::lock_guard<std::mutex>
        lock(clientsMutex);

    for(auto &client : clients)
    {
	if (client != senderSocket)
	    send(client, msg.c_str(), msg.size(), 0);
    }
}

void removeClient(int clientSocket) {
    std::lock_guard<std::mutex> lock(clientsMutex);

    clients.erase(
	    std::remove(clients.begin(), clients.end(), clientSocket),
	    clients.end());
}

void handleClient(int clientSocket) {
    char buffer[1024];
    while (true) {
	int bytes = recv(clientSocket, buffer, sizeof(buffer), 0);

	if (bytes <= 0) {
	    std::cout << "Client disconnected\n";
	    break;
	}

	buffer[bytes] = '\0';

	std::string msg(buffer);

	std::cout << "Client: " << buffer << '\n';

	broadcast(msg, clientSocket);
    }

    removeClient(clientSocket);
    close(clientSocket);
}

int main()
{
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);

    if (serverSocket < 0)
    {
        perror("socket");
        return 1;
    }

    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8080);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    if (bind(serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) < 0)
    {
        perror("bind");
        return 1;
    }

    if (listen(serverSocket, 10) < 0)
    {
        perror("listen");
        return 1;
    }

    std::cout << "Waiting for connection...\n";

    while (true)
	{
	    sockaddr_in clientAddr{};
	    socklen_t clientLen = sizeof(clientAddr);
	
	    int clientSocket = accept(serverSocket, (sockaddr*)&clientAddr, &clientLen);
	
	    if (clientSocket < 0)
	    {
	        perror("accept");
	        return 1;
	    }
	
	    std::cout << "Client connected!\n";

	    {
		std::lock_guard<std::mutex> lock(clientsMutex);
		clients.push_back(clientSocket);
	    }

	    std::thread t (
		handleClient,
		clientSocket
	    );

	    t.detach();
	}

    close(serverSocket);
}
