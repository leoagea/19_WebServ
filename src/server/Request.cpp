#include "Request.hpp"

Request::Request(std::string& fullRequest)
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

    //  BODY PARSING  //
    while (fullRequest[i] <= 32 || fullRequest[i] >= 126)
        i++;
    while (fullRequest[i]) {
        _body.append(fullRequest[i], 1);
        i++;
    }
}

Request::~Request() {}


std::string Request::getStartLine() { return _start_line; }

std::string Request::getHeader() { return _header; }

std::string Request::getBody() { return _body; }