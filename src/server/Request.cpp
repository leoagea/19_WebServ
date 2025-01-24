#include "Request.hpp"

Request::Request(std::string& fullRequest) : _empty("\r\n")
{
	int i = -1;
	while (fullRequest[++i] != '\r')
		_start_line.append(1, fullRequest[i]);
	++i;
	int count = 0;
	while (true)
	{
		if (fullRequest[i] != '\r' && fullRequest[i] != '\n')
			count = 0;
		_header.append(1, fullRequest[i]);

		if (fullRequest[i] == '\r'){
			if (++count == 2)
				break;
		}
		i++;
	}
}


std::string readRequest(int clientSocket)
{
    char buffer[1024];
    std::string request;
    ssize_t bytesRead;

    while ((bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0)) > 0)
    {
        request.append(buffer, bytesRead);
        if (request.find("\r\n\r\n") != std::string::npos)
        {
            break;
        }
    }
    size_t contentLengthPos = request.find("Content-Length:");
    if (contentLengthPos != std::string::npos)
    {
        size_t start = request.find("\r\n\r\n") + 4;
        size_t contentLength = std::stoi(request.substr(contentLengthPos + 15));
        while (request.size() - start < contentLength)
        {
            bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0);
            request.append(buffer, bytesRead);
        }
    }

    return request;
}

void parseHttpRequest(const std::string &rawRequest, std::string &method, std::string &url, std::string &body) {
    std::istringstream requestStream(rawRequest);
    std::string line;
    bool isBody = false;

    while (std::getline(requestStream, line)) {
        if (!line.empty() && line.back() == '\r') {
            line.pop_back();
        }

        if (isBody) {
        
            body += line + "\n"; 
        } else if (line.empty()) {
        
            isBody = true;
        } else if (method.empty() && url.empty()) {
            // Première ligne : méthode et URL
            std::istringstream firstLine(line);
            firstLine >> method >> url;
        }
    }
    if (!body.empty() && body.back() == '\n') {
        body.pop_back();
    }
}

std::string Request::getMethod()
{
    int i = 0;
    std::string str;

    while (_start_line[i] != ' ')
    {
        str.append(1, _start_line[i]);
        i++;
    }
    return str;
}

std::string Request::getStartLine() { return _start_line; }

std::string Request::getHeader()    { return _header; }

std::string Request::getBody()      { return _body; }


Request::~Request() {}