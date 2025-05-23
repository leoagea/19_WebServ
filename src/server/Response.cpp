#include "Response.hpp"
#include <sstream>
#include <iostream>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

void Response::m_delete(const std::string &fileName)
{
    std::string uploadDir = "uploadFolder/";
    std::string fullPath = uploadDir + fileName;
    struct stat buffer;
    if (stat(fullPath.c_str(), &buffer) != 0)
    {
        std::cerr << "File not found: " << fullPath << std::endl;
        _response = "HTTP/1.1 404 Not Found\r\n"
                    "Content-Type: text/plain\r\n"
                    "Keep-Alive: timeout=75\r\n"
                    "Content-Length: 14\r\n"
                    "\r\n"
                    "File not found";
        return;
    }
    if (remove(fullPath.c_str()) < 0)
    {
        std::cerr << "Failed to remove file: " << fullPath << std::endl;
        _response = "HTTP/1.1 500 Internal Server Error\r\n"
                    "Keep-Alive: timeout=75\r\n"
                    "Connection: keep-alive\r\n"
                    "Content-Length: 21\r\n"
                    "\r\n"
                    "Failed to delete file";
        return;
    }
    _response = "HTTP/1.1 200 OK\r\n"
                "Content-Type: text/plain\r\n"
                "Keep-Alive: timeout=75\r\n"
                "Content-Length: 25\r\n"
                "Connection: keep-alive\r\n"
                "\r\n"
                "File deleted successfully";
}

Response::Response()
    : _statusCode("200"), _statusMessage("OK"), _body(""), _contentType("text/html; charset=UTF-8"), _contentDisposition(""){}

// gestion des status
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

void Response::setContentDisposition(const std::string &disposition)
{
    _contentDisposition = disposition;
}

std::string Response::generateResponse(t_user &user)
{
    std::string response;

    response += "HTTP/1.1 " + _statusCode + " " + _statusMessage + "\r\n";
    response += "Location: \r\n";
    response += "Content-Type: " + _contentType + "\r\n";
    response += "Content-Length: " + _contentLength + "\r\n";
    if (!_contentDisposition.empty())
    {
        response += "Content-Disposition: " + _contentDisposition + "\r\n";
    }
    response += "Connection: keep-alive\r\n";
    response += "Keep-Alive: timeout=75\r\n";
    if (!user.login.empty())
        response += "Set-Cookie: login=" + user.login + "; Path=/\r\n";
    if (!user.sessionID.empty())
        response += "Set-Cookie: sessionID=" + user.sessionID + "; Path=/\r\n";
    if (!user.counter.empty())
        response += "Set-Cookie: counter=" + user.counter + "; Path=/\r\n";
    response += "\r\n";
    response += _body;

    return response;
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

void Response::get(const std::string &filePath, bool getBool, TcpServer &server)
{
    std::string fileContent = readFile(filePath);
    if (!fileContent.empty())
    {
        setBody(fileContent);

        size_t lastSlash = filePath.find_last_of("/\\");
        std::string filename = (lastSlash != std::string::npos) ? 
                             filePath.substr(lastSlash + 1) : filePath;
        if (filePath.find(".jpg") != std::string::npos || filePath.find(".jpeg") != std::string::npos)
        {
            setContentType("image/jpeg");
            if (!getBool)
                setContentDisposition(filename);
        }
        else if (filePath.find(".png") != std::string::npos)
        {
            setContentType("image/png");
            if (!getBool)
                setContentDisposition(filename);
        }
        else
        {
            setContentType("text/html; charset=UTF-8");
            if (!getBool)
                setContentDisposition(filename);
        }

        // Set Content-Length
        std::stringstream ss;
        ss << fileContent.size();
        _contentLength = ss.str();
        
        setStatusCode(200);
    }
    else if (!getBool)
    {
        server.clearIsIndex();
        server.handleClient(server.getClientFd());
    }
}

bool Response::isDirectoryWritable(const std::string &directory)
{
    struct stat dirInfo;
    if (stat(directory.c_str(), &dirInfo) != 0 || !S_ISDIR(dirInfo.st_mode))
    {
        return false;
    }
    return access(directory.c_str(), W_OK) == 0;
}

std::string Response::extractBoundary(const std::string &requestData)
{
    size_t pos = requestData.find("boundary=");
    if (pos == std::string::npos)
    {
        return "";
    }
    size_t endPos = requestData.find("\r\n", pos);
    return "--" + requestData.substr(pos + 9, endPos - (pos + 9));
}

std::string removeLastTwoLines(const std::string& content) {
    std::string result = content;
    size_t firstNewline = result.rfind('\n');

    if (firstNewline != std::string::npos) {
        size_t secondNewline = result.rfind('\n', firstNewline - 1);
        if (secondNewline != std::string::npos) {
            result.resize(secondNewline);
        } else {
            result.clear();
        }
    } else {
        result.clear();
    }
    return result;
}

bool Response::extractFileData(const std::string &requestData, const std::string &boundary, std::string &filename, std::string &fileContent)
{
    size_t boundaryPos = requestData.find(boundary);
    if (boundaryPos == std::string::npos)
    {
        return false;
    }

    size_t fileHeaderPos = requestData.find("\r\n\r\n", boundaryPos);
    if (fileHeaderPos == std::string::npos)
    {
        return false;
    }
    size_t fileStartPos = fileHeaderPos + 4;
    size_t fileEndPos = requestData.find(boundary, fileStartPos);
    if (fileEndPos == std::string::npos)
    {
        return false;
    }
    size_t filenamePos = requestData.find("filename=\"", boundaryPos);
    if (filenamePos == std::string::npos)
    {
        return false;
    }
    size_t filenameStart = filenamePos + 10;
    size_t filenameEnd = requestData.find("\"", filenameStart);
    if (filenameEnd == std::string::npos)
    {
        return false;
    }
    filename = requestData.substr(filenameStart, filenameEnd - filenameStart);
    fileContent = requestData.substr(fileStartPos, fileEndPos - fileStartPos - 2);
    fileContent = removeLastTwoLines(fileContent);
    return true;
}

void createUploadFolder(const std::string &folderName = "uploadFolder")
{
    struct stat info;

    if (stat(folderName.c_str(), &info) != 0)
        mkdir(folderName.c_str(), 0777);
}

void Response::post(const std::string &requestData)
{
    std::string uploadFolder = "./uploadFolder";
    createUploadFolder();

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
