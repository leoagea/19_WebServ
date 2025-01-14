/* TCP SERVER CLASS IMPLEMENTATION */
#include "TcpServer.hpp"
#include "../CGI/CgiHandler.hpp"
#include "Request.hpp"
#include "Response.hpp"
#include <fstream>

const std::string   readpage(std::ifstream &file)
{
    std::string line;
    std::string ret = "";

    while (getline(file, line))
    {
        line += "\n";
        ret += line;
    }

    return ret;
}

TcpServer::TcpServer(const std::vector<int> & ports) : _ports(ports)
{
    setupSocket();

    std::cout << G << IT << "Server initialized on port(s): ";

    for (size_t i = 0; i < _ports.size(); ++i) 
    {
        std::cout << _ports[i];
        if (i < _ports.size() - 1)
            std::cout << ", ";
    }
    std::cout << RES << std::endl;
}

TcpServer::~TcpServer()
{
    for (size_t i = 0; i < _serverSockets.size(); ++i) 
    {
        if (_serverSockets[i] != -1)
            close(_serverSockets[i]);
    }
    std::cout << G << IT << "Server closed successfully" << RES << std::endl;
}

void    TcpServer::setupSocket()
{
    for (size_t i = 0; i < _ports.size(); ++i) 
    {
        int serverSocket = socket(AF_INET, SOCK_STREAM, 0); /* AF_INET : IPv4, SOCK_STREAM : Socket de connexion, 0 : Protocole TCP */

        if (serverSocket < 0)
        {
            std::cerr << R << IT << "Socket " << i << ": creation failed" << RES << std::endl;
            exitCloseFds(_serverSockets);
        }

        int opt = 1;

        if (setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
        {
            std::cerr << R << IT << "Set sock option failed" << RES << std::endl;
            exitCloseFds(_serverSockets);
        }

        sockaddr_in  serverAddr;
        serverAddr.sin_family      = AF_INET;
        serverAddr.sin_port        = htons(_ports[i]);  /* Conversion en big-endian */
        serverAddr.sin_addr.s_addr = htonl(INADDR_ANY); /* Ecoute sur toutes les interfaces reseau */
        
        if (bind(serverSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0)
        {
            std::cerr << R << IT << "Binding of the socket to an ip address and a port failed" << RES << std::endl;
            exitCloseFds(_serverSockets);
        }

        if (listen(serverSocket, SOMAXCONN) < 0)
        {
            std::cerr << R << IT << "Listening of the socket failed" << RES << std::endl;
            exitCloseFds(_serverSockets);
        }

        makeNonBlocking(serverSocket);
        
        /* Ajouter le socket serveur au tableau de poll */
        struct pollfd serverPollFd;

        serverPollFd.fd     = serverSocket;
        serverPollFd.events = POLLIN;

        _pollFds.push_back(serverPollFd);
        _serverSockets.push_back(serverSocket);
    }
}

void    TcpServer::makeNonBlocking(int socket)
{
    if (fcntl(socket, F_SETFL, O_NONBLOCK) < 0) 
    {
        std::cerr << R << IT << "Activation of non-blocking mode failed" << RES << std::endl;
        exit(1);
    }
}

void    TcpServer::startServer()
{
    std::cout << B << IT << "Server is running...\n" << RES << std::endl;

    while (true)
    {
        int pollCount = poll(&_pollFds[0], (nfds_t)(_pollFds.size()), -1);

        if (pollCount < 0) 
        {
            std::cerr << R << IT << "Poll failed" << RES << std::endl;
            exitCloseFds(_serverSockets);
        }

        for (size_t i = 0; i < _pollFds.size(); ++i)
        {
            if (_pollFds[i].revents & POLLIN)
            {
                bool isServerSocket = false;

                for (size_t j = 0; j < _serverSockets.size(); ++j)
                {
                    if (_pollFds[i].fd == _serverSockets[   j])
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

void    TcpServer::acceptNewClient(int serverSocket)
{
    int clientFd = accept(serverSocket, NULL, NULL);

    if (clientFd < 0) 
    {
        std::cerr << R << IT << "Accept failed" << RES << std::endl;
        return;
    }
    std::cout << G << IT << "New client connected on file descriptor " << clientFd << RES << std::endl;

    makeNonBlocking(clientFd);

    struct pollfd clientPollFd;

    clientPollFd.fd     = clientFd;
    clientPollFd.events = POLLIN;

    _pollFds.push_back(clientPollFd);
}

//à fusionner avec le parsing, request et path à modif
std::string TcpServer::extractRequestedPath(const std::string &request)
{
    size_t methodEnd = request.find(' ');
    if (methodEnd == std::string::npos)
        return "";

    size_t pathStart = methodEnd + 1;
    size_t pathEnd = request.find(' ', pathStart);
    if (pathEnd == std::string::npos)
        return "";

    return request.substr(pathStart, pathEnd - pathStart);
}


//implémenter le rep racine
std::string TcpServer::resolvePath(const std::string &requestedPath)
{
    const std::string rootDirectory = "var/www/html";
    if (requestedPath.empty() || requestedPath == "/")
        return rootDirectory + "/index.html";

    return rootDirectory + requestedPath;
}


bool fileExists(const std::string& path) {
    return access(path.c_str(), F_OK) != -1;
}

void TcpServer::handleClient(int clientFd) {
    char buffer[REQUEST_HTTP_SIZE];
    int bytesRead = recv(clientFd, buffer, sizeof(buffer) - 1, 0);

    if (bytesRead <= 0) {
        std::cerr << "Client: " << clientFd << " disconnected or error" << std::endl;
        close(clientFd);
        cleanupClient(clientFd);

        return;
    }

    buffer[bytesRead] = '\0';
    std::string bufferStr = buffer;

    Response response;
    if (bufferStr.find("POST") == 0) {
        // Extraire le corps de la requête
        size_t headerEnd = bufferStr.find("\r\n\r\n");
        if (headerEnd != std::string::npos) {
            std::string body = bufferStr.substr(headerEnd + 4);
            response.post(body);
        } else {
            response.setStatusCode(400);
            response.setBody("<h1>400 Bad Request</h1>");
        }
    } 
    else if (bufferStr.find("GET") == 0) {
        std::string requestedPath = extractRequestedPath(bufferStr);
        std::string fullPath = resolvePath(requestedPath);
        response.get(fullPath);
    } 
    else {
        response.setStatusCode(405);
        response.setBody("<h1>405 Method Not Allowed</h1>");
    }

    std::string fullResponse = response.generateResponse();
    send(clientFd, fullResponse.c_str(), fullResponse.size(), 0);
}



// void TcpServer::handleClient(int clientFd) {
//     char buffer[REQUEST_HTTP_SIZE];
//     int bytesRead = recv(clientFd, buffer, sizeof(buffer) - 1, 0);
//     if (bytesRead <= 0) {
//         std::cerr << "Client: " << clientFd << " disconnected or error" << std::endl;
//         close(clientFd);
//         cleanupClient(clientFd);
//         return;
//     }

//     buffer[bytesRead] = '\0';
//     std::string bufferStr = buffer;
//     Response response;
//     response.parseRequest(bufferStr);

//     // à changer avec le parsing de leo
//     if (1){
//         std::string requestedPath = extractRequestedPath(bufferStr);
//         std::string fullPath = resolvePath(requestedPath);

//         response.get(fullPath);
//     }
//     else {
//         response.setStatusCode(405); // Method Not Allowed
//         response.setBody("<h1>405 Method Not Allowed</h1>");
//     }
//     std::string fullResponse = response.generateResponse();
//     send(clientFd, fullResponse.c_str(), fullResponse.size(), 0);
// }


void TcpServer::cleanupClient(int fd)
{
    for (size_t i = 0; i < _pollFds.size(); ++i) 
    {
        if (_pollFds[i].fd == fd) 
        {
            _pollFds.erase(_pollFds.begin() + i);
            break;
        }
    }
}

void    TcpServer::exitCloseFds(std::vector<int> &serverSockets)
{
    std::vector<int>::iterator it = serverSockets.begin();

    for (; it != serverSockets.end(); ++it) 
        close(*it);

    exit(1);
}

uint16_t    TcpServer::getSocketPort(int socket)
{
    sockaddr_in sin;
    socklen_t   socklen = sizeof(sin);

    if (getsockname(socket, (struct sockaddr *)&sin, &socklen) < 0) 
    {
        std::cout << "Getsockname failed" << std::endl;
        return 0;
    }

    return htons(sin.sin_port);
}
	
void	TcpServer::generateLog(std::string color, const std::string& message, const char *logType)
{
    std::time_t now = std::time(nullptr);
    std::tm* local_time = std::localtime(&now);

    std::cout << color << "[" << logType << "] ";
    std::cout << (local_time->tm_year + 1900) << "-";

    if (local_time->tm_mon + 1 < 10)  
        std::cout << '0' << (local_time->tm_mon + 1) << "-";
    else
        std::cout << (local_time->tm_mon + 1) << "-";
    if (local_time->tm_mday < 10)
        std::cout << '0' << local_time->tm_mday << ' ';
    else
        std::cout << local_time->tm_mday << ' ';
    if (local_time->tm_hour < 10)
        std::cout << '0' << local_time->tm_hour << ":";
    else
        std::cout << local_time->tm_hour << ":";
    if (local_time->tm_min < 10)
        std::cout << '0' << local_time->tm_min << ":";
    else
        std::cout << local_time->tm_min << ":";
    if (local_time->tm_sec < 10)
        std::cout << '0' << local_time->tm_sec;
    else
        std::cout << local_time->tm_sec;

    std::cout << " => "<< message << RES << std::endl;	
}

std::vector<int> TcpServer::getServerSockets() { return _serverSockets; }

std::vector<int> TcpServer::getPorts()         { return _ports; }

std::vector<pollfd> TcpServer::getPollFds()    { return _pollFds; }

sockaddr_in TcpServer::getServerAddress()      { return _serverAddress; }
