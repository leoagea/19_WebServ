// Response.hpp
#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include <string>
#include <fstream>
#include <map>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <sys/stat.h>
#include <unistd.h>
#include <cstdio>

#include "Request.hpp"
#include "../../inc/Enum.h"
#include "../../inc/Struct.h"
#include "Cookies.hpp"
#include "TcpServer.hpp"

class Cookies;
class TcpServer;

class Response
{
private:
    std::string _statusCode;
    std::string _statusMessage;
    std::string _body;
    std::string _contentType;
    std::string _contentLength;
    std::string _contentDisposition;
    bool _keepAlive;
    const char *_path;
    std::string _response;
    std::string _method;      // méthode HTTP (GET, POST, DELETE)
    std::string _url;         // à voir comment setup
    std::string _httpVersion; // Version HTTP (HTTP/1.1, HTTP/2, etc.)? askip il faut
    std::map<std::string, std::string> _headers;
    const int _method_int;

public:
    Response();
    ~Response();
    void logRequest(const std::string &clientIP, const std::string &requestLine, const std::string &statusCode);
    void parseRequest(const std::string &rawRequest);
    void setStatusCode(int code);
    void setBody(const std::string &body);
    void setContentType(const std::string &type);
    void setContentDisposition(const std::string &disposition);
    void setKeepAlive(bool keepAlive);
    
    std::string generateResponse(t_user &);
    std::string extractHeaders(const std::string &fullResponse);
    void get(const std::string &filePath, bool getBool, TcpServer &server);
    static std::string readFile(const std::string &filePath);
    void sendRedirect(int clientFd, const std::string &newUrl, std::string prefix);
    void m_delete(const std::string &filePath);
    bool extractFileData(const std::string &requestData, const std::string &boundary, std::string &filename, std::string &fileContent);
    std::string extractBoundary(const std::string &requestData);
    bool isDirectoryWritable(const std::string &directory);
    void post(const std::string &requestData);
    int getMethod();
    int getBodySize();
};

#endif
