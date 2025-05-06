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
    std::string _response;

public:
    Response();
    ~Response();
    void setStatusCode(int code);
    void setBody(const std::string &body);
    void setContentType(const std::string &type);
    void setContentDisposition(const std::string &disposition);
    
    std::string generateResponse(t_user &);
    void get(const std::string &filePath, bool getBool, TcpServer &server);
    static std::string readFile(const std::string &filePath);
    void m_delete(const std::string &filePath);
    bool extractFileData(const std::string &requestData, const std::string &boundary, std::string &filename, std::string &fileContent);
    std::string extractBoundary(const std::string &requestData);
    bool isDirectoryWritable(const std::string &directory);
    void post(const std::string &requestData);
};

#endif
