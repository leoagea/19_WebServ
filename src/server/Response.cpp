#include "Response.hpp"
#include <sstream>
#include <iostream>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

Response::Response()
    : _statusCode("200"), _statusMessage("OK"), _body(""), _contentType("text/html; charset=UTF-8"), _keepAlive(true)
{
}

//gestion des status
void Response::setStatusCode(int code)
{
    std::stringstream status;
    status << code;
    _statusCode = status.str();

    switch (code)
    {
    case 200:
        _statusMessage = "OK";
        break;
    case 404:
        _statusMessage = "Not Found";
        break;
    case 405:
        _statusMessage = "Method Not Allowed";
        break;
    default:
        _statusMessage = "Internal Server Error";
        break;
    }
}

void Response::setBody(const std::string &body)
{
    _body = body;
    _contentLength = std::to_string(body.size());
}

void Response::setContentType(const std::string &type)
{
    _contentType = type;
}

void Response::setKeepAlive(bool keepAlive)
{
    _keepAlive = keepAlive;
}

//rep générée
std::string Response::generateResponse()
{
    std::string response;
    response += "HTTP/1.1 " + _statusCode + " " + _statusMessage + "\r\n";
    response += "Content-Type: " + _contentType + "\r\n";
    response += "Content-Length: " + _contentLength + "\r\n";
    response += "Connection: " + std::string(_keepAlive ? "keep-alive" : "close") + "\r\n";
    response += "\r\n";
    response += _body;

    return response;
}

//post
void Response::parseRequest(const std::string &rawRequest)
{
    std::istringstream stream(rawRequest);
    std::string line;
	//lecture des headers
    if (std::getline(stream, line))
    {
        std::istringstream lineStream(line);
        lineStream >> _method >> _url >> _httpVersion;
    }
	//entete http
    while (std::getline(stream, line))
    {
        if (line == "\r" || line.empty())
            break;

        // Extraction la clé + value
        size_t colonPos = line.find(':');
        if (colonPos != std::string::npos)
        {
            std::string key = line.substr(0, colonPos);
            std::string value = line.substr(colonPos + 1);

            // clear les espaces
            key.erase(0, key.find_first_not_of(" \t"));
            key.erase(key.find_last_not_of(" \t") + 1);
            value.erase(0, value.find_first_not_of(" \t"));
            value.erase(value.find_last_not_of(" \t") + 1);

            _headers[key] = value;
        }
    }
    // Lire le corps de la requête s'il exitse?
    // std::string body;
    // while (std::getline(stream, line))
    //     body += line + "\n";
    // if (!body.empty())
    //     _body = body;
    // 
}

//extract pour print que le headers (check)
std::string Response::extractHeaders(const std::string &fullResponse)
{
    std::istringstream stream(fullResponse);
    std::string line;
    std::string headers;

    while (std::getline(stream, line))
    {
        if (line == "\r" || line.empty())
            break;

        headers += line + "\n";
    }

    return headers;
}

std::string Response::readFile(const std::string &filePath)
{
    std::ifstream file(filePath);
    if (!file.is_open())
    {
        return "";
    }

    std::string content, line;
    while (std::getline(file, line))
    {
        content += line + "\n";
    }

    return content;
}

void Response::get(const std::string &filePath)
{
    std::string fileContent = readFile(filePath);
    
    if (!fileContent.empty())
    {
        setBody(fileContent);
        setStatusCode(200);
        setContentType("text/html; charset=UTF-8");
    }
    else
    {
        setBody("<h1>404 Not Found</h1>");
        setStatusCode(404);
        setContentType("text/html; charset=UTF-8");
    }
}
