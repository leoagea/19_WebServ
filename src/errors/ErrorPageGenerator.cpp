/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ErrorPageGenerator.cpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lagea <lagea@student.s19.be>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/02 14:48:53 by lagea             #+#    #+#             */
/*   Updated: 2025/01/03 15:08:01 by lagea            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ErrorPageGenerator.hpp"

ErrorPageGnerator::ErrorPageGnerator()
{
}

ErrorPageGnerator::ErrorPageGnerator(ServerBlock &server)
{
    _errors = server.getErrorPagesMap();   
}

ErrorPageGnerator::~ErrorPageGnerator()
{
}

//Generate the html code of the error page if error code is handled
//Otherwise if error code is not accepted, throw an error
std::string ErrorPageGnerator::generateErrorPageCode(int error)
{
    if (_errors.find(error) == _errors.end())
        throw std::runtime_error("Error page not handled");
    
    if (_errors[error].empty()){
        switch (error)
        {
            case 400:
                generateHtmlCode(400, "Bad Request");
                break;
    
            case 403:
                generateHtmlCode(403, "Forbidden");
                break;

            case 404:
                generateHtmlCode(404, "Not Found");
                break;

            case 405:
                generateHtmlCode(405, "Method Not allowed");
                break;

            case 408:
                generateHtmlCode(408, "Request Timeout");
                break;

            case 413:
                generateHtmlCode(413, "Payload too large");
                break;
                
            case 500:
                generateHtmlCode(500, "Internal Server Error");
                break;
        }
    }
    else
    {
        _htmlcode = readHtmlCode(_errors[error]);
    }

    return _htmlcode;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void ErrorPageGnerator::generateHtmlCode(int error, std::string msg)
{
    std::stringstream ss;
    ss << error;

    std::string num = ss.str();
    
    _htmlcode = "";
    _htmlcode = "<!DOCTYPE html>\n\
<html>\n\
<head>\n\
\t<meta charset=\"utf-8\">\n\
\t<title>408 Request Timeout</title>\n\
</head>\n\
<body>\n\
\t<h1>";

    _htmlcode += num + " - " + msg + "</h1>\n";
    _htmlcode += "</body>\n\
</html>";
    
}

std::string ErrorPageGnerator::readHtmlCode(std::string &path)
{
    if (!PathChecking::exist(path) || !PathChecking::isFile(path)){
        generateHtmlCode(404, "Not Found");
        return _htmlcode;
    }
    if (!PathChecking::getReadPermission(path)){
        generateHtmlCode(403, "Forbidden");
        return _htmlcode;
    }
        
    std::ifstream file(path.c_str());
    if (!file.is_open()){
        generateHtmlCode(404, "Not Found");
        return _htmlcode;
    }
    
    _htmlcode = "";
    std::string line;
    while (getline(file, line))
       _htmlcode += line;
    
    return _htmlcode;
}