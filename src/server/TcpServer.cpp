/* TCP SERVER CLASS IMPLEMENTATION */
#include "TcpServer.hpp"

TcpServer::TcpServer(const std::vector<int> & ports) : _ports(ports)
{
    setupSocket();

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
        int serverSocket = socket(AF_INET, SOCK_STREAM, 0); /* AF_INET : IPv4, SOCK_STREAM : Socket de connexion, 0 : Protocole TCP */
        if (serverSocket < 0)
        {
            std::cerr << BRED << ITALIC << "Socket " << i << ": creation failed" << RESET << std::endl;
            exitCloseFds(_serverSockets);
        }
        int opt = 1;
        if (setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
        {
            std::cerr << BRED << ITALIC << "Set sock option failed" << RESET << std::endl;
            exitCloseFds(_serverSockets);
        }

        struct sockaddr_in serverAddr;
        serverAddr.sin_family = AF_INET;
        serverAddr.sin_port = htons(_ports[i]);  /* Conversion en big-endian */
        serverAddr.sin_addr.s_addr = htonl(INADDR_ANY); /* Ecoute sur toutes les interfaces reseau */
        
        if (bind(serverSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0)
        {
            std::cerr << BRED << ITALIC << "Binding of the socket to an ip address and a port failed" << RESET << std::endl;
            exitCloseFds(_serverSockets);
        }

        if (listen(serverSocket, SOMAXCONN) < 0)
        {
            std::cerr << BRED << ITALIC << "Listening of the socket failed" << RESET << std::endl;
            exitCloseFds(_serverSockets);
        }

        makeNonBlocking(serverSocket);
        
        /* Ajouter le socket serveur au tableau de poll() */
        struct pollfd serverPollFd;

        serverPollFd.fd = serverSocket;
        serverPollFd.events = POLLIN;
        _pollFds.push_back(serverPollFd);
        _serverSockets.push_back(serverSocket);
    }
}

void TcpServer::makeNonBlocking(int socket)
{
    if (fcntl(socket, F_SETFL, O_NONBLOCK) < 0) {
        std::cerr << BRED << ITALIC << "Activation of non-blocking mode failed" << RESET << std::endl;
        exit(1);
    }
}

void TcpServer::startServer()
{
    std::cout << BBLUE << ITALIC << "Server is running..." << RESET << std::endl;
    while (true) {
        int pollCount = poll(&_pollFds[0], _pollFds.size(), -1);
        if (pollCount < 0) {
            std::cerr << BRED << ITALIC << "Poll failed" << RESET << std::endl;
            exitCloseFds(_serverSockets);
        }
        for (int i = 0; i < _pollFds.size(); ++i)
        {
            if (_pollFds[i].revents & POLLIN)
            {
                bool isServerSocket = false;
                for (int j = 0; j < _serverSockets.size(); ++j)
                {
                    if (_pollFds[i].fd == _serverSockets[j])
                    {
                        acceptNewClient(_serverSockets[j]);
                        isServerSocket = true;
                        break;
                    }
                }
                if (isServerSocket == false) 
                    handleClient(_pollFds[i].fd);
            }
        }
    }
}

void TcpServer::acceptNewClient(int serverSocket)
{
    int clientFd = accept(serverSocket, NULL, NULL);
    if (clientFd < 0) {
        std::cerr << BRED << ITALIC << "Accept failed" << RESET << std::endl;
        return;
    }

    std::cout << BGREEN << ITALIC << "New client connected on file descriptor " << clientFd << RESET << std::endl;

    makeNonBlocking(clientFd);

    struct pollfd clientPollFd;
    
    clientPollFd.fd = clientFd;
    clientPollFd.events = POLLIN;
    _pollFds.push_back(clientPollFd);
}

void TcpServer::handleClient(int clientFd)
{
    char buffer[4096];
    int bytesRead = recv(clientFd, buffer, sizeof(buffer) - 1, 0);

    if (bytesRead <= 0) {
        std::cerr << BRED << ITALIC << "Client disconnected" << RESET << std::endl;
        close(clientFd);
        cleanupClient(clientFd);
        return;
    }
    buffer[bytesRead] = '\0';
    std::cout << "Received: " << buffer << " from fd " << clientFd << " on port " << std::endl;
    // std::string response = "HTTP/1.1 200 OK\r\nContent-Length: 13\r\n\r\nHello, World!";
    // send(clientFd, response.c_str(), response.size(), 0);
}

void TcpServer::cleanupClient(int fd)
{
    for (int i = 0; i < _pollFds.size(); ++i) {
        if (_pollFds[i].fd == fd) {
            _pollFds.erase(_pollFds.begin() + i);
            break;
        }
    }
}

void TcpServer::exitCloseFds(std::vector<int> &serverSockets)
{
    std::vector<int>::iterator it = serverSockets.begin();
    for (; it != serverSockets.end(); ++it) {
        close(*it);
    }
    exit(1);
}

std::vector<int> TcpServer::getServerSockets() { return _serverSockets; }

std::vector<int> TcpServer::getPorts() { return _ports; }

std::vector<pollfd> TcpServer::getPollFds() { return _pollFds; }

sockaddr_in TcpServer::getServerAddress() { return _serverAddress; }