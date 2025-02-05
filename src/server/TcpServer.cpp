/* TCP SERVER CLASS IMPLEMENTATION */
#include "TcpServer.hpp"
#include "../CGI/CgiHandler.hpp"
#include "Request.hpp"
#include "Response.hpp"
#include "DirectoryListing.hpp"
#include <fstream>
#include <iostream>
#include <string>
#include <map>


const std::string   readpage(std::ifstream &file)
{
    std::string line;
    std::string ret = "";

    while (getline(file, line))
    {
        line += "\n";
        ret  += line;
    }

    return ret;
}

TcpServer::TcpServer(const std::vector<int> & ports, const ConfigFile &config, std::map<std::string, std::string> envMap) : _ports(ports), _clientMap(), _config(config), _envMap(envMap), _previousUser("")
{
    setupSocket();
    std::cout << IT << "Server initialized on port(s): ";

    for (size_t i = 0; i < _ports.size(); ++i) 
    {
        std::cout << _ports[i];
        if (i < _ports.size() - 1)
            std::cout << ", ";
    }
    std::cout << RES << "\n" << std::endl;
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
            std::cerr << R << IT << "Binding of the socket to an ip address and a port failed : "<< errno << RES << std::endl;
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

t_mapDB readDB(const std::string &path)
{
    std::string line;
    t_mapDB map;
    std::ifstream file(path.c_str());

    if (!file.is_open()){
        TcpServer::generateLog(RED, "Failed to open cookies DB", "ERROR");
        return map;
    }

    while(getline(file, line)){
        size_t first = line.find('|');
        size_t second = line.find('|', first + 1);
        t_user user;
        if (first != std::string::npos && second != std::string::npos){
            user.login = line.substr(0, first);
            user.sessionID = line.substr(first + 1, second - first - 1);
            user.counter = line.substr(second + 1, line.size());
            map[user.login] = user;
        }
    }

    return map;
}

void    TcpServer::startServer()
{
    TcpServer::generateLog(BLUE, "Server is running...", "INFO");
    _cookiesMap = readDB(COOKIE_DB_PATH);

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

void    TcpServer::acceptNewClient(int serverSocket)
{
    int clientFd = accept(serverSocket, NULL, NULL);

    if (clientFd < 0) 
    {
        std::cerr << R << IT << "Accept failed" << RES << std::endl;
        return;
    }
    _clientMap[clientFd] = getServerBlockBySocket(serverSocket);

    TcpServer::generateLog(BLUE, "New client connected", "INFO");
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

std::string TcpServer::getFullUrl(const std::string& requestBuffer) {
    size_t methodEnd = requestBuffer.find(' ');
    if (methodEnd == std::string::npos) {
        return "";
    }
    std::string method = requestBuffer.substr(0, methodEnd);
    size_t pathStart = methodEnd + 1;
    size_t pathEnd = requestBuffer.find(' ', pathStart);
    if (pathEnd == std::string::npos) {
        return "";
    }
    std::string path = requestBuffer.substr(pathStart, pathEnd - pathStart);
    // std::string host = "";
    // size_t hostHeaderStart = requestBuffer.find("\nHost: ");
    // if (hostHeaderStart != std::string::npos) {
    //     hostHeaderStart += 7;
    //     size_t hostHeaderEnd = requestBuffer.find('\r', hostHeaderStart);
    //     if (hostHeaderEnd != std::string::npos) {
    //         host = requestBuffer.substr(hostHeaderStart, hostHeaderEnd - hostHeaderStart);
    //     }
    // }
    std::string fullUrl = path;
    return fullUrl;
}

std::string extractCookiesFromRequest(const std::string &request)
{
    std::string cookie = "Cookie: ";
    size_t start = request.find(cookie, 0) + cookie.size();
    size_t end = request.find('\r', start);
    std::string line = request.substr(start, end - start);

    return line;
}

std::string trim(const std::string &str)
{
    const char* WHITESPACE = " \t\n\r\f\v";

    std::string::size_type start = str.find_first_not_of(WHITESPACE);
    if (start == std::string::npos) {
        return "";
    }

    std::string::size_type end = str.find_last_not_of(WHITESPACE);

    return str.substr(start, end - start + 1);
}

std::map<std::string, std::string> parseUrlParameters(const std::string& url) {
    std::map<std::string, std::string> params;
    size_t questionMarkPos = url.find('?');
    if (questionMarkPos == std::string::npos) {
        return params;
    }
    std::string queryString = url.substr(questionMarkPos + 1);
    size_t start = 0;
    while (start < queryString.length()) {
        size_t equalPos = queryString.find('=', start);
        size_t ampPos = queryString.find('&', start);
        if (equalPos == std::string::npos) {
            break;
        }
        std::string key = queryString.substr(start, equalPos - start);
        std::string value = queryString.substr(equalPos + 1, ampPos - equalPos - 1);
        params[key] = value;
        if (ampPos == std::string::npos) {
            break;
        }
        start = ampPos + 1;
    }
    return params;
}


std::string removeExtraSlashes(const std::string &path) {
    std::string cleanedPath;
    bool lastWasSlash = false;
    char ch;
    for (int i=0; i < (int)path.size(); i++) {
        ch = path[i];
        if (ch == '/') {
            if (!lastWasSlash) {
                cleanedPath += ch;
            }
            lastWasSlash = true;
        } else {
            cleanedPath += ch;
            lastWasSlash = false;
        }
    }

    return cleanedPath;
}

std::string extractFileToDelete(const std::string &url)
{
    std::string param = "file-to-delete=";
    size_t pos = url.find(param);
    
    if (pos == std::string::npos)
        return "";

    return url.substr(pos + param.length());
}

std::string TcpServer::getDirectoryFromFirstLine(const std::string & method, const std::string & fullUrl)
{
    return method + " " + fullUrl;
}


std::string removeQueryString(const std::string & url) {
    size_t questionMarkPos = url.find('?');
    if (questionMarkPos != std::string::npos) {
        return url.substr(0, questionMarkPos);
    }
    return url;
}

//implémenter le rep racine
std::string TcpServer::resolvePath(const std::string &requestedPath, int clientFd)
{
    const std::string rootDirectory = _clientMap[clientFd].getRootDir();
    if (requestedPath.empty() || requestedPath == "/")
        return _clientMap[clientFd].getIndex();
    return rootDirectory + requestedPath;
}

bool fileExists(const std::string& path) { return access(path.c_str(), F_OK) != -1; }


void TcpServer::handleClient(int clientFd) 
{
    CgiHandler cgi(_envMap);
    
    char buffer[REQUEST_HTTP_SIZE];
    int bytesRead = recv(clientFd, buffer, sizeof(buffer) - 1, 0);
    if (bytesRead <= 0) 
    {
        TcpServer::generateLog(BLUE, "A client has disconnected", "INFO");
        // Erase clientFd from clientMap
        close(clientFd);
        cleanupClient(clientFd);
        return;
    }
    buffer[bytesRead] = '\0';
    Response response;
    std::string bufferStr = buffer;
    std::string requestBuffer = buffer;
    std::string fullUrl = getFullUrl(requestBuffer);
    std::map<std::string, std::string> params;
    std::string deleteFile;
    
    // std::cout << bufferStr << std::endl;
    fullUrl = removeQueryString(fullUrl);

    std::string urlPath = extractRequestedPath(bufferStr);
    if (bufferStr.find("POST ") == 0 || bufferStr.find("GET ") == 0)
        params = parseUrlParameters(urlPath);
    if (bufferStr.find("DELETE ") == 0)
        deleteFile = extractFileToDelete(urlPath);
    urlPath = removeQueryString(urlPath);
    removeExtraSlashes(fullUrl);
    removeExtraSlashes(urlPath);
    std::string requestedPath = urlPath;
    std::string rootPath;
    try
    {
        if (requestedPath == _clientMap[clientFd].getLocationBlockByString(requestedPath).getUri() ) {
            requestedPath = _clientMap[clientFd].getLocationBlockByString(requestedPath).getIndexLoc();
            rootPath = _clientMap[clientFd].getRootDir();
        }
    }
    catch(const std::exception& e) {}

    std::string fullPath = resolvePath(requestedPath, clientFd);
    
    t_user user;
    int getBool = 0;
   	int postBool = 0;
    int deleteBool= 0;
    std::vector<s_info> listing;

    try
    {
        locationBlock location = _clientMap[clientFd].getLocationBlockByString(urlPath);
        

        if (location.getAutoIndexLoc()) {
            listing = DirectoryListing::listDirectory(rootPath);
            response.setBody(DirectoryListing::generateDirectoryListingHTML(rootPath, listing));
        }
        else {
            try
            {
                getBool = location.getAllowedMethodGET();

                if (location.getCgi() && location.getCgiScriptName() != ""){
                    std::string ext = std::strrchr(location.getCgiScriptName().c_str(), '.');
                    if (ext == ".go"){
                        std::string dir = std::getenv("PWD") + std::string("/") + location.getRootDirLoc() + std::string("wiki");
                        cgi.executego(dir);
                    }
                }
 
                if (params.find("login") != params.end()){
                    std::string cookies = extractCookiesFromRequest(bufferStr);
                    user = TcpServer::parseCookies(clientFd, cookies, params["login"]);
                    if (_cookiesMap.find(user.login) == _cookiesMap.end()){
                        user.sessionID = Cookies::generateSessionID();
                        user.counter = "0";
                        _cookiesMap[user.login] = user;
                    }
                    else{
                        user.sessionID = _cookiesMap[user.login].sessionID;
                        if (_previousUser != user.login){
                            user.counter = _cookiesMap[user.login].counter;
                            _previousUser = user.login;
                        }
                    }
                }

                response.get(fullPath, getBool);

                if (bufferStr.find("POST ") != 0 && bufferStr.find("DELETE ") != 0)
                    TcpServer::generateLog(BLUE, getDirectoryFromFirstLine("GET", fullUrl), "INFO");

				if (bufferStr.find("POST ") == 0) {
                    response.m_delete(deleteFile);
					size_t headerEnd = bufferStr.find("\r\n\r\n");
					if (headerEnd == std::string::npos) {
                        TcpServer::generateLog(RED, getDirectoryFromFirstLine("POST", fullUrl), "ERROR");
						response.setStatusCode(400);
						response.setBody("<h1>400 Bad Request</h1>");
					}
					else {
						std::string body = bufferStr.substr(headerEnd + 4);
						postBool = location.getAllowedMethodPOST();
						if (!postBool) {
                            TcpServer::generateLog(RED, getDirectoryFromFirstLine("POST", fullUrl), "ERROR");
							response.setStatusCode(405);
							response.setBody("<h1>405 Method Not Allowed</h1>");
						}
                        else if (bufferStr.find("DELETE ") == 0) {
                            headerEnd = bufferStr.find("\r\n\r\n");
                            if (headerEnd == std::string::npos) {
                                TcpServer::generateLog(RED, getDirectoryFromFirstLine("DELETE", fullUrl), "ERROR");
                                response.setStatusCode(400);
                                response.setBody("<h1>400 Bad Request</h1>");
                            }
                            else {
                                body = bufferStr.substr(headerEnd + 4);
                                deleteBool = _clientMap[clientFd].getLocationBlockByString(urlPath).getAllowedMethodDELETE();
                                if (!deleteBool) {
                                    response.setStatusCode(405);
                                    response.setBody("<h1>405 Method Not Allowed</h1>");
                                }
                                else {
                                    TcpServer::generateLog(BLUE, getDirectoryFromFirstLine("DELETE", fullUrl), "INFO");
                                    response.m_delete(deleteFile);
                                }
                            }
                        }
						else {
                            TcpServer::generateLog(BLUE, getDirectoryFromFirstLine("POST", fullUrl), "INFO");
                            if (location.getCgi() && params.find("min-price") != params.end() && params.find("max-price") != params.end()){
                                uint minPrice = std::atoi(params["min-price"].c_str());
                                uint maxPrice = std::atoi(params["max-price"].c_str());
                                cgi.setMinPrice(minPrice);
                                cgi.setMaxPrice(maxPrice);
                                std::string dir(std::getenv("PWD"));
                                cgi.setCurrentDir(dir += "/");
                                cgi.executepy(dir += location.getCgiPath());
                            }
                            else
                            {
							    response.post(body);
                            }
            			}
					}
				}
				else if (bufferStr.find("DELETE ") == 0) {
					size_t headerEnd = bufferStr.find("\r\n\r\n");
					if (headerEnd == std::string::npos) {
                        TcpServer::generateLog(RED, getDirectoryFromFirstLine("DELETE", fullUrl), "ERROR");
						response.setStatusCode(400);
						response.setBody("<h1>400 Bad Request</h1>");
					}
					else {
						std::string body = bufferStr.substr(headerEnd + 4);
						deleteBool = location.getAllowedMethodDELETE();
						if (!deleteBool) {
							response.setStatusCode(405);
							response.setBody("<h1>405 Method Not Allowed 2</h1>");
						}
						else {
                            TcpServer::generateLog(BLUE, getDirectoryFromFirstLine("DELETE", fullUrl), "INFO");
							// response.m_delete();
            			}
					}
				}
            }
            catch(const std::exception& e)
            { 
                TcpServer::generateLog(RED, getDirectoryFromFirstLine("GET", fullUrl), "ERROR");
                response.setStatusCode(400);
                response.setBody("<h1>400 Bad Request</h1>");
            }
        }
    }
    catch(const std::exception& e)
    {
        TcpServer::generateLog(RED, getDirectoryFromFirstLine("GET", fullUrl), "ERROR");
        response.setStatusCode(400);
        response.setBody("<h1>400 Bad Request 1</h1>");
    }

    _cookiesMap[user.login] = user;
    Cookies::writeCookiesInDB(_cookiesMap);
    std::string fullResponse = response.generateResponse(user);
    send(clientFd, fullResponse.c_str(), fullResponse.size(), 0);
}

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

t_user    TcpServer::parseCookies(int clientFd, const std::string &line, const std::string &login)
{
    std::stringstream ss(line);
    std::string pairStr;
    t_user user;
    (void) clientFd;

    while (getline(ss, pairStr, ';')){
        pairStr = trim(pairStr);
        size_t pos = pairStr.find('=');
        if (pos != std::string::npos){
            std::string name = pairStr.substr(0, pos);
            std::string val = pairStr.substr(pos + 1, pairStr.size() - pos);
            if (name == "sessionID")
                user.sessionID = val;
            else if (name == "counter")
                user.counter = val;
        }
    }

    user.login = login;
    return user;
}

//Return the serverblock associate with the socket
//Otherwise throw a runtime error
ServerBlock TcpServer::getServerBlockBySocket(int socket)
{
    int port = static_cast<int>(getSocketPort(socket));
    std::vector<ServerBlock> servVect = _config.getServerBlockVector();

    std::vector<ServerBlock>::iterator it;
    for (it = servVect.begin(); it != servVect.end(); it++)
        if (port == it->getListeningPort()) {
            break;
        }
    return *it;
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
    std::time_t now = std::time(NULL);
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