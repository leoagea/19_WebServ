#ifndef __REQUEST__
# define __REQUEST__

# include "TcpServer.hpp"

class Request 
{
    public :
        Request(std::string&fullRequest);
        ~Request();

        std::string getStartLine();
        std::string getHeader();
        std::string getBody();

    private :
        std::string _start_line;
        std::string _header;
        std::string _empty = "\r\n";
        std::string _body;
};


#endif