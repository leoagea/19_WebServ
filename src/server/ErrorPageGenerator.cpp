/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ErrorPageGenerator.cpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lagea <lagea@student.s19.be>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/02 14:48:53 by lagea             #+#    #+#             */
/*   Updated: 2025/02/05 18:32:52 by lagea            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ErrorPageGenerator.hpp"

std::string generateHtmlCode(int error, std::string msg)
{
    std::stringstream ss;
    ss << error;

    std::string num = ss.str();
    std::string htmlcode;
    
    htmlcode = "";
    htmlcode = "<!DOCTYPE html>\n\
<html>\n\
<head>\n\
\t<meta charset=\"utf-8\">\n\
\t<title>408 Request Timeout</title>\n\
</head>\n\
<body>\n\
\t<h1>";

    htmlcode += num + " - " + msg + "</h1>\n";
    htmlcode += "</body>\n\
</html>";
    
    return htmlcode;
}

std::string readHtmlCode(std::string &path)
{
    std::string html;
    
    if (!PathChecking::exist(path) || !PathChecking::isFile(path)){
        generateHtmlCode(404, "Not Found");
        return html;
    }
    if (!PathChecking::getReadPermission(path)){
        generateHtmlCode(403, "Forbidden");
        return html;
    }
        
    std::ifstream file(path.c_str());
    if (!file.is_open()){
        generateHtmlCode(404, "Not Found");
        return html;
    }
    
    html = "";
    std::string line;
    while (getline(file, line))
       html += line;
    
    return html;
}

//Generate the html code of the error page if error code is handled
//Otherwise if error code is not accepted, throw an error
std::string ErrorPageGenerator::generateErrorPageCode(std::map<int, std::string> &errormap, int error)
{
    if (errormap.find(error) == errormap.end()){
        std::stringstream ss;
        ss << "Error page " << error << " not handled";
        throw std::runtime_error(ss.str());
    }
    
    std::string html;
    if (errormap[error].empty()){
        switch (error)
        {
            case 400:
                html = generateHtmlCode(400, "Bad Request");
                break;
    
            case 403:
                html = generateHtmlCode(403, "Forbidden");
                break;

            case 404:
                html = generateHtmlCode(404, "Not Found");
                break;

            case 405:
                html = generateHtmlCode(405, "Method Not allowed");
                break;

            case 408:
                html = generateHtmlCode(408, "Request Timeout");
                break;

            case 413:
                html = generateHtmlCode(413, "Payload too large");
                break;
                
            case 500:
                html = generateHtmlCode(500, "Internal Server Error");
                break;
        }
    }
    else
    {
        html = readHtmlCode(errormap[error]);
    }

    return html;
}
