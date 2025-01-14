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

// void	Response::m_post()
// {	
// 	if (open(const_cast<char *>(_path), O_CREAT, 0777) < 0)
// 	{

// 	}		
// }

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

// void Response::parseRequest(const std::string &rawRequest)
// {
//     std::istringstream stream(rawRequest);
//     std::string line;
// 	//lecture des headers
//     if (std::getline(stream, line))
//     {
//         std::istringstream lineStream(line);
//         lineStream >> _method >> _url >> _httpVersion;
//     }
// 	//entete http
//     while (std::getline(stream, line))
//     {
//         if (line == "\r" || line.empty())
//             break;

//         // Extraction la clé + value
//         size_t colonPos = line.find(':');
//         if (colonPos != std::string::npos)
//         {
//             std::string key = line.substr(0, colonPos);
//             std::string value = line.substr(colonPos + 1);

//             // clear les espaces
//             key.erase(0, key.find_first_not_of(" \t"));
//             key.erase(key.find_last_not_of(" \t") + 1);
//             value.erase(0, value.find_first_not_of(" \t"));
//             value.erase(value.find_last_not_of(" \t") + 1);

//             _headers[key] = value;
//         }
//     }
//     // Lire le corps de la requête s'il exitse?
//     // std::string body;
//     // while (std::getline(stream, line))
//     //     body += line + "\n";
//     // if (!body.empty())
//     //     _body = body;
//     // 
// }

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
        setBody("<h1>404 Not Fo8und</h1>");
        setStatusCode(404);
        setContentType("text/html; charset=UTF-8");
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
        setBody("<h1>500 Internal Server Error</h1><p>Le répertoire de sauvegarde n'est pas accessible.</p>");
        return;
    }
    std::string boundary = extractBoundary(requestData);
    std::string filename, fileContent;
    if (!extractFileData(requestData, boundary, filename, fileContent))
    {
        setStatusCode(400);
        setBody("<h1>400 Bad Request</h1><p>Échec lors de l'extraction des données du fichier.</p>");
        return;
    }
    size_t dotPos = filename.find_last_of('.');
    if (dotPos == std::string::npos || filename.substr(dotPos) != ".txt")
    {
        setStatusCode(400);
        setBody("<h1>400 Bad Request</h1><p>Seuls les fichiers texte (.txt) sont autorisés.</p>");
        return;
    }
    std::string filePath = uploadFolder + "/" + filename;
    std::ofstream outputFile(filePath, std::ios::binary);
    if (!outputFile.is_open())
    {
        setStatusCode(500);
        setBody("<h1>500 Internal Server Error</h1><p>Impossible de sauvegarder le fichier.</p>");
        return;
    }
    outputFile.write(fileContent.c_str(), fileContent.size());
    outputFile.close();

    setStatusCode(201);
    setBody("<h1>201 Created</h1><p>Fichier texte sauvegardé avec succès : " + filename + "</p>");
}



//pour print comme serv python si on veut, faut juste rajouter le clienp et status dans tcp
void Response::logRequest(const std::string& clientIP, const std::string& requestLine, const std::string& statusCode) {
    auto now = std::chrono::system_clock::now();
    std::time_t now_time = std::chrono::system_clock::to_time_t(now);
    std::tm tm = *std::localtime(&now_time);



    std::ostringstream oss;
    oss << std::put_time(&tm, "%d/%b/%Y %H:%M:%S");

    std::string dateTime = oss.str();
    std::cout << clientIP << " - - [" << dateTime << "] \"" << requestLine << "\" " << statusCode << " -" << std::endl;
}

Response::~Response() {}
