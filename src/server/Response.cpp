#include "Response.hpp"

Response::Response() : _method(DELETE), _path("var/www/html/deletableFile") {}

Response::~Response() {}

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

int	Response::getMethod() { return _method; }
