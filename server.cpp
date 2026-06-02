#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

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

    if (bind(serverSocket,
             (sockaddr*)&serverAddr,
             sizeof(serverAddr)) < 0)
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

    sockaddr_in clientAddr{};
    socklen_t clientLen = sizeof(clientAddr);

    int clientSocket =
        accept(serverSocket,
               (sockaddr*)&clientAddr,
               &clientLen);

    if (clientSocket < 0)
    {
        perror("accept");
        return 1;
    }

    std::cout << "Client connected!\n";

    while (true)
	{
	    char buffer[1024];

	    int bytes =
	        recv(clientSocket,
	             buffer,
	             sizeof(buffer)-1,
	             0);

	    if (bytes <= 0)
	        break;

	    buffer[bytes] = '\0';

	    std::cout << "Client: "
	              << buffer
	              << '\n';

	    std::string reply;

	    std::getline(std::cin, reply);

	    send(clientSocket,
	         reply.c_str(),
	         reply.size(),
	         0);
	}

    close(clientSocket);
    close(serverSocket);
}
