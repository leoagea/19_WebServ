// Response.hpp
#ifndef RESPONSE_HPP
# define RESPONSE_HPP

# include <string>
# include <fstream>
# include <map>
# include <iostream>
# include <iomanip>
# include <sstream>
# include <sys/stat.h>
# include <unistd.h>

# include <cstdio>
# include "Request.hpp"
# include "../../inc/Enum.h"

// enum
// {
//     GET    = 1,
//     POST   = 2,
//     DELETE = 3
// };

// class Response
// {
//     public :
//         Response();
//         ~Response();

//         void            m_delete();
// 	    void		    m_post();
// 	    void		    m_get();

//         int             getMethod();

//     private :
//         const int       _method_int;
//         const char*     _path;
// 	    std::string	_response;
// };

class Response
{
private:
    std::string _statusCode;
    std::string _statusMessage;
    std::string _body;
    std::string _contentType;
    std::string _contentLength;
    bool _keepAlive;
    const char*  _path;
    std::string	_response;
    std::string _method;                    // méthode HTTP (GET, POST, DELETE)
    std::string _url;                        // à voir comment setup
    std::string _httpVersion;                // Version HTTP (HTTP/1.1, HTTP/2, etc.)? askip il faut
    std::map<std::string, std::string> _headers;
    const int   _method_int;

public:
    Response();
    ~Response();
	void logRequest(const std::string& clientIP, const std::string& requestLine, const std::string& statusCode);
	void parseRequest(const std::string &rawRequest);
    void setStatusCode(int code);
    void setBody(const std::string &body);
    void setContentType(const std::string &type);
    void setKeepAlive(bool keepAlive);
    std::string generateResponse();
	std::string extractHeaders(const std::string &fullResponse);
    void get(const std::string &filePath, bool getBool);
	static std::string readFile(const std::string &filePath);
    void sendRedirect(int clientFd, const std::string &newUrl, std::string prefix);

	bool extractFileData(const std::string& requestData, const std::string& boundary, std::string& filename, std::string& fileContent);
	std::string extractBoundary(const std::string& requestData);
	bool isDirectoryWritable(const std::string& directory);
	void post(const std::string& requestData);
    void            m_delete();
	int             getMethod();


};

#endif
