#ifndef __REQUEST__
#define __REQUEST__

#include "TcpServer.hpp"

#include <string>
#include <sstream>
#include <iostream>
#include <sys/socket.h>
#include <iostream>
#include <string>

class Request
{
public:
    Request(std::string &fullRequest);
    std::string readRequest(int clientSocket);
    void processHttpRequest(int clientSocket);
    void parseHttpRequest(const std::string &rawRequest, std::string &method, std::string &url, std::string &body);
    void sendResponse(int clientFd, const std::string &httpResponse);
    // Dans Request.hpp
    void handleRequest(const std::string &method, const std::string &url, const std::string &body, int clientFd);

    ~Request();

    std::string getStartLine();
    std::string getHeader();
    std::string getBody();

    std::string getMethod();

private:
    std::string _start_line;
    std::string _header;
    std::string _empty;
    std::string _body;
};

#endif