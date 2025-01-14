#include "Request.hpp"

Request::Request(std::string& fullRequest)
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
        // Suppression du caractère de retour chariot
        if (!line.empty() && line.back() == '\r') {
            line.pop_back();
        }

        if (isBody) {
        
            body += line + "\n";    //  corps de la requête
        } else if (line.empty()) {
        
            isBody = true;    // si Ligne vide == début du corps
        } else if (method.empty() && url.empty()) {
            // Première ligne : méthode et URL
            std::istringstream firstLine(line);
            firstLine >> method >> url;
        }
    }

    // Suppression du dernier saut de ligne dans le corps
    if (!body.empty() && body.back() == '\n') {
        body.pop_back();
    }
}

void processHttpRequest(int clientSocket)
{
    std::string rawRequest = readRequest(clientSocket);

    // Parsing url et corps
    std::string method, url, body;
    parseHttpRequest(rawRequest, method, url, body);

    // Gestion de la requête
   // handleRequest(method, url, body);
}


void sendResponse(int clientFd, const std::string& httpResponse) {
    ssize_t bytesSent = send(clientFd, httpResponse.c_str(), httpResponse.size(), 0);
    if (bytesSent == -1) {
        std::cerr << "Error sending response to client!" << std::endl;
    } else {
        std::cout << "Sent " << bytesSent << " bytes to client." << std::endl;
    }
}

void handleRequest(const std::string &method, const std::string &url, const std::string &body)
{
    Response response;

    if (method == "POST" && url == "/post_text.html")
    {
        response.post(body);
        std::string httpResponse = response.generateResponse();
    }
    else
    {
        response.setStatusCode(404);
        response.setBody("<h1>404 Not Found</h1>");
        std::string httpResponse = response.generateResponse();
    }
}



	Request::~Request() {}

	std::string Request::getStartLine() { return _start_line; }

	std::string Request::getHeader() { return _header; }

	std::string Request::getBody() { return _body; }