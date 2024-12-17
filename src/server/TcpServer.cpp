/* TCP SERVER CLASS IMPLEMENTATION */
#include "TcpServer.hpp"

TcpServer::TcpServer(const std::vector<int> & ports) : _ports(ports)
{
    this->setupSocket();

    std::cout << BGREEN << ITALIC << "Server initialized on port(s): ";

    for (int i = 0; i < _ports.size(); ++i) {
        std::cout << _ports[i];
        if (i < _ports.size() - 1)
            std::cout << ", ";

    }

    std::cout << RESET << std::endl;
}

TcpServer::~TcpServer()
{
    for (int i = 0; i < _serverSockets.size(); ++i) {
        if (_serverSockets[i] != -1)
            close(_serverSockets[i]);
    }
    std::cout << BGREEN << ITALIC << "Server closed successfully" << RESET << std::endl;
}

void TcpServer::setupSocket()
{
    for (int i = 0; i < _ports.size(); ++i) {
        int serverSocket = socket(AF_INET, SOCK_STREAM, 0); // AF_INET : IPv4, SOCK_STREAM : Socket de connexion, 0 : Protocole TCP
        if (serverSocket < 0)
        {
            std::cerr << BRED << ITALIC << "Socket " << i << ": creation failed" << RESET << std::endl;
            exit(1);
        }
        int opt = 1;
        if (setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
        {
            std::cerr << RED << ITALIC << "Set sock option failed" << RESET << std::endl;
            exit(1);
        }

        struct sockaddr_in serverAddr;
        serverAddr.sin_family = AF_INET;
        serverAddr.sin_addr.s_addr = INADDR_ANY; // Ecoute sur toutes les interfaces reseau
        serverAddr.sin_port = htons(_ports[i]);  // Conversion en big-endian
        
        if (bind(serverSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0)
        {
            std::cerr << RED << ITALIC << "Binding of the socket to an ip address and a port failed" << RESET << std::endl;
            exit(1);
        }

        if (listen(serverSocket, SOMAXCONN) < 0)
        {
            std::cerr << RED << ITALIC << "Liatening of the socket failed" << RESET << std::endl;
            exit(1);
        }
        // makeNonBlocking(serverSocket);
        
        /* Ajouter le socket serveur au tableau de poll() */
        struct pollfd serverPollFd;
        serverPollFd.fd = serverSocket;
        serverPollFd.events = POLLIN;
        _pollFds.push_back(serverPollFd);
        _serverSockets.push_back(serverSocket);
    }
} 

std::vector<int> TcpServer::getServerSockets()
{
    return _serverSockets;
}