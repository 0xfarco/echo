#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

int main()
{
    int clientSocket = socket(AF_INET, SOCK_STREAM, 0);

    if (clientSocket < 0)
    {
        perror("socket");
        return 1;
    }

    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8080);

    inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr);

    if (connect(clientSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) < 0)
    {
        perror("connect");
        return 1;
    }

    std::cout << "Connected to server!\n";

    while (true)
	{
	    std::string msg;
	
	    std::getline(std::cin, msg);
	
	    send(clientSocket, msg.c_str(), msg.size(), 0);
	
	    char buffer[1024];
	
	    int bytes = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
	
	    buffer[bytes] = '\0';
	
	    std::cout << "Server: " << buffer << '\n';
	}

    close(clientSocket);
}
