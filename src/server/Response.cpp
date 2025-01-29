#include "Response.hpp"
#include <sstream>
#include <iostream>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

void	 Response::m_delete()
{
    std::cout << _path << std::endl;
    if (remove(_path) < 0)
    {
        std::cerr << "Failed to remove file with delete method" << std::endl;
	    _response =  "HTTP/1.1 500 Internal Server Error\r\n"
                     "Content-Type: text/plain\r\n"
                     "Content-Length: 21\r\n"
                     "\r\n"
                     "Failed to delete file";
	    return;
    }
    _response = "HTTP/1.1 200 OK\r\n"
                "Content-Type: text/plain\r\n"
                "Content-Length: 19\r\n"
                "Connection: keep-alive\r\n"
                "Keep-Alive: timeout=60\r\n"
                "\r\n"
                "File deleted successfully";
}

int	Response::getMethod() { return _method_int; }

Response::Response()
    :  _statusCode("200"), _statusMessage("OK"), _body(""), _contentType("text/html; charset=UTF-8"), _keepAlive(true),_method_int(DELETE) {}

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
    std::stringstream ss;
    ss << body.size();
    _contentLength = ss.str();
}

void Response::setContentType(const std::string &type)
{
    _contentType = type;
}

void Response::setKeepAlive(bool keepAlive)
{
    _keepAlive = keepAlive;
}

void Response::sendRedirect(int clientFd, const std::string &requestedPath, std::string prefix) {
    std::string newUrl = requestedPath;

    if (newUrl.find(prefix) == 0) {
        newUrl = "/" + newUrl.substr(prefix.length());
    }
    newUrl = newUrl;
    std::cout << "path 2 " << requestedPath << std::endl;
    std::string response = 
        "HTTP/1.1 302 Found\r\n"
        "Location: " + newUrl + "\r\n"
        "Content-Length: 0\r\n"
        "\r\n";
    
    send(clientFd, response.c_str(), response.size(), 0);
}

std::string Response::generateResponse()
{
    std::string response;
    if (_body.find("myapp") != std::string::npos) {
        size_t pos = _body.find("myapp");
        while (pos != std::string::npos) {
            _body.replace(pos, 5, "delete.html");
            pos = _body.find("myapp", pos + 1);
        }
    }

    response += "HTTP/1.1 " + _statusCode + " " + _statusMessage + "\r\n";
    response += "Location: oui \r\n";
    response += "Content-Type: " + _contentType + "\r\n";
    response += "Content-Length: " + _contentLength + "\r\n";
    response += "Connection: keep-alive\r\n";
    response += "Keep-Alive: timeout=75\r\n";
    response += "\r\n";
    response += _body;

    return response;
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
    std::ifstream file(filePath.c_str());
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

void Response::get(const std::string &filePath, bool getBool)
{
    std::string fileContent = readFile(filePath);

    if (!fileContent.empty())
    {
        setBody(fileContent);
        setStatusCode(200);
        setContentType("text/html; charset=UTF-8");
    }
    else if (!getBool)
    {
        throw std::runtime_error("get error");
    }
}

bool Response::isDirectoryWritable(const std::string& directory) {
    struct stat dirInfo;
    if (stat(directory.c_str(), &dirInfo) != 0 || !S_ISDIR(dirInfo.st_mode)) {
        return false;
    }
    return access(directory.c_str(), W_OK) == 0;
}


std::string Response::extractBoundary(const std::string& requestData) {
    size_t pos = requestData.find("boundary=");
    if (pos == std::string::npos) {
        return "";
    }
    size_t endPos = requestData.find("\r\n", pos);
    return "--" + requestData.substr(pos + 9, endPos - (pos + 9));
}


bool Response::extractFileData(const std::string& requestData, const std::string& boundary, std::string& filename, std::string& fileContent) {
    size_t boundaryPos = requestData.find(boundary);
    if (boundaryPos == std::string::npos) {
        return false;
    }

    size_t fileHeaderPos = requestData.find("\r\n\r\n", boundaryPos);
    if (fileHeaderPos == std::string::npos) {
        return false;
    }
    size_t fileStartPos = fileHeaderPos + 4;

    size_t fileEndPos = requestData.find(boundary, fileStartPos);
    if (fileEndPos == std::string::npos) {
        return false;
    }

    // Extraction des informations d'en-tête du fichier
    size_t filenamePos = requestData.find("filename=\"", boundaryPos);
    if (filenamePos == std::string::npos) {
        return false;
    }
    size_t filenameStart = filenamePos + 10;
    size_t filenameEnd = requestData.find("\"", filenameStart);
    if (filenameEnd == std::string::npos) {
        return false;
    }
    filename = requestData.substr(filenameStart, filenameEnd - filenameStart);

    // Extraction du contenu du fichier
    fileContent = requestData.substr(fileStartPos, fileEndPos - fileStartPos - 2);
    return true;
}

void Response::post(const std::string &requestData)
{
    std::string uploadFolder = "./uploadFolder";
    if (!isDirectoryWritable(uploadFolder))
    {
        setStatusCode(500);
        setBody(_body + "<p><strong>Erreur :</strong> Le répertoire de sauvegarde n'est pas accessible.</p>");
        return;
    }

    std::string boundary = extractBoundary(requestData);
    std::string filename, fileContent;
    if (!extractFileData(requestData, boundary, filename, fileContent))
    {
        setStatusCode(400);
        setBody(_body + "<p><strong>Erreur :</strong> Échec lors de l'extraction des données du fichier.</p>");
        return;
    }

    size_t dotPos = filename.find_last_of('.');
    if (dotPos == std::string::npos || filename.substr(dotPos) != ".txt")
    {
        setStatusCode(400);
        setBody(_body + "<p><strong>Erreur :</strong> Seuls les fichiers texte (.txt) sont autorisés.</p>");
        return;
    }

    std::string filePath = uploadFolder + "/" + filename;
    std::ofstream outputFile(filePath.c_str(), std::ios::binary);
    if (!outputFile.is_open())
    {
        setStatusCode(500);
        setBody(_body + "<p><strong>Erreur :</strong> Impossible de sauvegarder le fichier.</p>");
        return;
    }

    outputFile.write(fileContent.c_str(), fileContent.size());
    outputFile.close();
    setStatusCode(201);
    _body += "<p><strong>Succès :</strong> Fichier texte sauvegardé avec succès : " + filename + "</p>";
}


Response::~Response() {}
