#include "Request.hpp"

Request::Request(std::string& fullRequest, std::string &htmlPage) : _body(htmlPage)
{
    //  FIRST LINE PARSING  //
    int i = -1;
    while (fullRequest[++i] != '\r')
        _start_line.append(1, fullRequest[i]);
    ++i;

    //  HEADER PARSING  //
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

Request::~Request() {}

std::string Request::getStartLine() { return _start_line; }

std::string Request::getHeader() { return _header; }

std::string Request::getBody() { return _body; }