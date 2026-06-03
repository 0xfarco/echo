#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <thread>

void receiveMessages(int clientSocket)
{
    char buffer[1024];

    while (true)
    {
        int bytes = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);

        if (bytes <= 0)
        {
            std::cout << "Disconnected from server\n";
            break;
        }

        buffer[bytes] = '\0';

        std::cout << "\nMessage: " << buffer << '\n';
    }
}

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

    std::thread receiver(receiveMessages, clientSocket);

    while (true)
	{
	    std::string msg;
	
	    std::getline(std::cin, msg);
	
	    send(clientSocket, msg.c_str(), msg.size(), 0);
	}

    close(clientSocket);

    receiver.join();

    return 0;
}
