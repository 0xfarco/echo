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

    inet_pton(AF_INET,
              "127.0.0.1",
              &serverAddr.sin_addr);

    if (connect(clientSocket,
                (sockaddr*)&serverAddr,
                sizeof(serverAddr)) < 0)
    {
        perror("connect");
        return 1;
    }

    std::cout << "Connected to server!\n";

    const char* msg = "Hello Server";

    send(clientSocket,
         msg,
         strlen(msg),
         0);

    close(clientSocket);
}
