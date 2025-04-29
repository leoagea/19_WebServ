/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   serverBlock.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lagea <lagea@student.s19.be>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 13:28:47 by lagea             #+#    #+#             */
/*   Updated: 2025/04/29 19:19:55 by lagea            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "serverBlock.hpp"

ServerBlock::ServerBlock() : _listeningports(), _servername(""), _rootdir(""), _index(""),
                             _bodysizelimit(-1), _host(""), _hostbytes(), _locationblock(), _errorpages(), _reportError()
{
}

ServerBlock::ServerBlock(int start, std::vector<t_token> &tokenVec, int *j, const ErrorReporter &reporter) : _listeningports(), _servername(""), _rootdir(""), _index(""),
                                                                                                             _bodysizelimit(-1), _host(""), _hostbytes(), _locationblock(), _errorpages(), _reportError(reporter)
{
    initializeMapErrorPages();
    parseAllServerVariables(start, tokenVec, j);
}

ServerBlock::~ServerBlock()
{
}

std::vector<int> ServerBlock::getListeningPorts() const
{
    return _listeningports;
}

int ServerBlock::getListeningPortByIndex(int index) const
{
    return _listeningports.at(index);
}

std::string ServerBlock::getServerName() const
{
    return _servername;
}

std::string ServerBlock::getRootDir() const
{
    return _rootdir;
}

std::string ServerBlock::getIndex() const
{
    return _index;
}

int ServerBlock::getBodySizeLimit() const
{
    return _bodysizelimit;
}

// Return path to file if redefined in conf file, return empty string otherwise
// If error number is not handle, return "nulll"
std::string ServerBlock::getErrorPagePath(int error) const
{
    std::map<int, std::string>::const_iterator it = _errorpages.find(error);

    if (it != _errorpages.end())
    {
        if (!it->second.empty())
            return it->second;
        else
            return "";
    }
    else
        return "null";
}

std::map<int, std::string> ServerBlock::getErrorPagesMap() const
{
    return _errorpages;
}

std::string ServerBlock::getHost() const
{
    return _host;
}

// Return vector of each bytes of host address
// index 0 is first byte
std::vector<int> ServerBlock::getHostBytesVector() const
{
    return _hostbytes;
}

// Return bytes by index of vector
// return -1 if out of bound, -2 if host not setup
int ServerBlock::getHostBytesByIndex(int index) const
{
    int n = -1;
    try
    {
        n = _hostbytes.at(index);
    }
    catch (std::exception &e)
    {
        return -1;
    }
    return n;
}

std::map<std::string, locationBlock> ServerBlock::getLocationBlockMap() const
{
    return _locationblock;
}

// return the object location if find, otherwise throw a runtime error
locationBlock ServerBlock::getLocationBlockByString(std::string &locationName) const
{
    std::map<std::string, locationBlock>::const_iterator it = _locationblock.find(locationName);
    if (it != _locationblock.end())
    {
        return (it->second);
    }
    else
    {
        throw std::runtime_error("");
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void ServerBlock::initializeMapErrorPages()
{
    _errorpages[400] = ""; // Bad request
    _errorpages[403] = ""; // Forbidden
    _errorpages[404] = ""; // Not found
    _errorpages[405] = ""; // Method not allowed
    _errorpages[408] = ""; // Request timeout
    _errorpages[413] = ""; // Payload too large
    _errorpages[500] = ""; // Internal server error
    _errorpages[504] = ""; // Gateway timeout
}

void ServerBlock::parseAllServerVariables(int startIndex, std::vector<t_token> &tokenVec, int *j)
{
    int i = 0;
    for (i = *j; i < (int)tokenVec.size() && tokenVec[i].type != closebracket; i++)
    {
        t_token token = tokenVec[i];
        if (token.type == keyword && token.value == "listen")
        {
            i++;
            if (!_listeningports.empty()){
                _reportError(i, "listening port already defined");
                while (tokenVec[i].type != semicolon)
                    i++;
            }
            while (tokenVec[i].type != semicolon)
            {
                parseListeningPort(tokenVec[i]);  
                i++;
            }
        }
        else if (token.type == keyword && token.value == "server_name")
        {
            parseServerName(tokenVec[++i]);
            i++;
        }
        else if (token.type == keyword && token.value == "root" && tokenVec[i + 2].type == semicolon)
        {
            parseRootDir(tokenVec[++i]);
            i++;
        }
        else if (token.type == keyword && token.value == "index" && tokenVec[i + 2].type == semicolon)
        {
            parseIndex(tokenVec[++i]);
            i++;
        }
        else if (token.type == keyword && token.value == "limit_body_size" && tokenVec[i + 2].type == semicolon)
        {
            parseLimitBodySize(tokenVec[++i]);
            i++;
        }
        else if (token.type == keyword && token.value == "error_page" && tokenVec[i + 3].type == semicolon)
        {
            parseErrorsPages(tokenVec[i + 1], tokenVec[i + 2]);
            i += 3;
        }
        else if (token.type == keyword && token.value == "host" && tokenVec[i + 2].type == semicolon)
        {
            parseHost(tokenVec[++i]);
            i++;
        }
        else if (token.type == keyword && token.value == "location" && tokenVec[i + 1].type == string && tokenVec[i + 2].type == openbracket)
        {
            int begin = i;
            std::vector<t_token>::iterator start = tokenVec.begin() + i;
            while (tokenVec[i].type != closebracket)
                i++;
            std::vector<t_token>::iterator end = tokenVec.begin() + i + 1;

            std::vector<t_token> tmp(start, end);
            if (_locationblock.find(tokenVec[begin + 1].value) == _locationblock.end())
            {
                locationBlock block(*this, tmp, _reportError);
                // std::cout << block << std::endl;
                _locationblock.insert(std::make_pair(tokenVec[begin + 1].value, block));
            }
            else
                _reportError(begin, "duplicate location");
        }
        else
        {
            char stop = 0;
            _reportError(i, "unknown token");
            while (tokenVec[i].type != semicolon || (stop == 0 || stop == 1))
            {
                if (tokenVec[i].type == openbracket)
                    stop = 1;
                else if (stop && tokenVec[i].type == closebracket)
                    break;
                i++;
            }
        }
    }

    if (i == 0)
        *j += 1;
    *j = i;

    std::vector<std::string> check = checkAllDefined();
    if (!check.empty())
    {
        std::string notdefined = "";
        for (std::vector<std::string>::iterator it = check.begin(); it != check.end(); it++)
        {
            notdefined += *it + " ";
        }
        notdefined += "not defined";
        _reportError(startIndex, notdefined);
    }
}

void ServerBlock::parseListeningPort(t_token &token)
{
    if (token.type == number)
    {
        int port = atoi(token.value.c_str());
        if (port <= 1023 || port > UINT16_MAX)
        {
            _reportError(token.index, "port range exceeded expected 1024 - 65535");
        }
        else
            _listeningports.push_back(port);
    }
    else
        _reportError(token.index, "expected number");
}

void ServerBlock::parseServerName(t_token &token)
{
    if (_servername == "")
    {
        if (token.type == string)
        {
            _servername = token.value;
        }
        else
            _reportError(token.index, "expected a string");
    }
    else
        _reportError(token.index, "server_name already defined");
}

void ServerBlock::parseRootDir(t_token &token)
{
    if (_rootdir == "")
    {
        if (token.type == string)
        {
            if (PathChecking::exist(token.value))
                if (PathChecking::isDirectory(token.value))
                    _rootdir = token.value;
                else
                    _reportError(token.index, "expected a directory path");
            else
                _reportError(token.index, "path does not exist");
        }
        else
            _reportError(token.index, "expected a path");
    }
    else
        _reportError(token.index, "root already defined");
}

void ServerBlock::parseIndex(t_token &token)
{
    if (_index == "")
    {
        if (token.type == string)
        {
            std::string path;

            if (_rootdir[_rootdir.size() - 1] == '/')
                path = _rootdir + token.value;
            else
                path = _rootdir + "/" + token.value;

            if (PathChecking::exist(path))
                if (PathChecking::isFile(path))
                    if (PathChecking::getReadPermission(path))
                        _index = path;
                    else
                        _reportError(token.index, "file has no read permission");
                else
                    _reportError(token.index, "expected a file");
            else
                _reportError(token.index, "does not exist");
        }
        else
            _reportError(token.index, "expected a file");
    }
    else
        _reportError(token.index, "index already defined");
}

void ServerBlock::parseLimitBodySize(t_token &token)
{
    if (_bodysizelimit == -1)
    {
        if (token.type == number)
        {
            int maxbodysize = atoi(token.value.c_str());
            if (maxbodysize <= 0 || maxbodysize >= 1025)
                _reportError(token.index, "max body size range exceeded 1 - 1024");
            else
                _bodysizelimit = maxbodysize;
        }
        else
            _reportError(token.index, "expected a number");
    }
    else
        _reportError(token.index, "limit_body_size already defined");
}

void ServerBlock::parseErrorsPages(t_token &num, t_token &token)
{
    if (num.type == number && token.type == string)
    {
        int numpage = atoi(num.value.c_str());
        if (_errorpages.find(numpage) != _errorpages.end())
        {
            if (_errorpages[numpage] == "")
            {
                std::string path = token.value;

                if (!PathChecking::isAbsolutePath(token.value))
                {
                    if (_rootdir[_rootdir.size() - 1] == '/')
                        path = _rootdir + token.value;
                    else
                        path = _rootdir + "/" + token.value;
                }

                if (PathChecking::exist(path))
                    if (PathChecking::isFile(path))
                        if (PathChecking::getReadPermission(path))
                            _errorpages[numpage] = path;
                        else
                            _reportError(token.index, "file has no read permission");
                    else
                        _reportError(token.index, "is not a file");
                else
                    _reportError(token.index, "not defined"
                                              "file does not exist");
            }
            else
                _reportError(token.index, "error page already defined");
        }
        else
            _reportError(token.index, "error page not handle");
    }
    else
        _reportError(token.index, "expected a number and a path");
}

void ServerBlock::parseHost(t_token &token)
{
    if (_host == "" && _hostbytes.empty())
    {
        if (token.type == number)
        {
            if (!isHostValid(token.value))
                _reportError(token.index, "expected IPv4 address");
            else
            {
                std::string n;
                std::stringstream ss(token.value);
                getline(ss, n, '.');
                _hostbytes.push_back(atoi(n.c_str()));
                getline(ss, n, '.');
                _hostbytes.push_back(atoi(n.c_str()));
                getline(ss, n, '.');
                _hostbytes.push_back(atoi(n.c_str()));
                getline(ss, n, '.');
                _hostbytes.push_back(atoi(n.c_str()));
                _host = token.value;
            }
        }
        else
            _reportError(token.index, "expected a ip address");
    }
    else
        _reportError(token.index, "host already defined");
}

bool ServerBlock::isHostValid(std::string &host)
{
    std::string token;
    std::stringstream ss(host);

    while (getline(ss, token, '.'))
    {
        int i = 0;
        while (token[i])
        {
            if (!isdigit(token[i]))
                return false;
            i++;
        }
        int n = atoi(token.c_str());
        if (n < 0 || n > 255)
            return false;
    }
    return true;
}

bool ServerBlock::isLogExtensionValid(std::string &path)
{
    std::string extension = ".log";
    std::string::iterator it = std::find_end(path.begin(), path.end(), extension.begin(), extension.end());

    if (it == path.end())
        return false;
    return true;
}

std::vector<std::string> ServerBlock::checkAllDefined()
{
    std::vector<std::string> notdefined;

    if (_listeningports.empty())
        notdefined.push_back("listen port");
    if (_servername == "")
        notdefined.push_back("servername");
    if (_rootdir == "")
        notdefined.push_back("root directory");
    if (_index == "")
        notdefined.push_back("index page");
    if (_bodysizelimit == -1)
        notdefined.push_back("body size limit");
    if (_host == "" && _hostbytes.empty())
        notdefined.push_back("host");

    return notdefined;
}

// Print every private attributes for debugging purpose
std::ostream &operator<<(std::ostream &out, const ServerBlock &obj)
{
    out << BLUE << "Server Block" << RESET << std::endl;

    out << CYAN << "Listening Port" << RESET << std::endl;
    out << "Port:  \n";
    for (int i = 0; i < (int)obj.getListeningPorts().size(); i++)
    {
        out << "     " << obj.getListeningPortByIndex(i) << "\n";
    }

    out << CYAN << "Server Name" << RESET << std::endl;
    out << "Name:  " << obj.getServerName() << std::endl;

    out << CYAN << "Root Dir" << RESET << std::endl;
    std::string pathRoot = obj.getRootDir();
    out << "Dir:  " << pathRoot << std::endl;
    out << "Exist: " << PathChecking::exist(pathRoot) << "  IsDir:  " << PathChecking::isDirectory(pathRoot)
        << "  R:  " << PathChecking::getReadPermission(pathRoot) << "  W:  " << PathChecking::getWritePermission(pathRoot) << "  X:  " << PathChecking::getExecPermission(pathRoot) << std::endl;

    out << CYAN << "Index" << RESET << std::endl;
    std::string pathIndex = obj.getIndex();
    out << "Dir:  " << pathIndex << std::endl;
    out << "Exist: " << PathChecking::exist(pathIndex) << "  IsFile:  " << PathChecking::isFile(pathIndex)
        << "  R:  " << PathChecking::getReadPermission(pathIndex) << "  W:  " << PathChecking::getWritePermission(pathIndex) << "  X:  " << PathChecking::getExecPermission(pathIndex) << std::endl;

    out << CYAN << "Body Size Limit" << RESET << std::endl;
    out << "Limit: " << obj.getBodySizeLimit() << std::endl;

    out << CYAN << "Error pages" << RESET << std::endl;
    std::map<int, std::string> map = obj.getErrorPagesMap();
    std::cout << "Iterate through map: " << std::endl;
    for (std::map<int, std::string>::iterator it = map.begin(); it != map.end(); it++)
    {
        std::cout << "  Error:  " << it->first << "   Path:  '" << it->second << "'" << std::endl;
    }
    std::cout << "Get page 400:  '" << obj.getErrorPagePath(400) << "'" << std::endl;
    std::cout << "Get page 404:  '" << obj.getErrorPagePath(404) << "'" << std::endl;
    std::cout << "Get page 1000:  '" << obj.getErrorPagePath(1000) << "'" << std::endl;

    out << CYAN << "Host" << RESET << std::endl;
    std::cout << "string:  " << obj.getHost() << std::endl;
    std::cout << "vector:  ";
    std::vector<int> vec = obj.getHostBytesVector();
    for (int i = 0; i < (int)vec.size(); i++)
        std::cout << vec[i] << "  ";
    std::cout << std::endl;
    std::cout << "Test: getHostBytesByIndex: index = -1, index = 0" << std::endl;
    std::cout << "index -1:  " << obj.getHostBytesByIndex(-1) << "   index 0:  " << obj.getHostBytesByIndex(0) << std::endl;

    std::cout << std::endl;
    // Location
    {
        std::map<std::string, locationBlock> tmp = obj.getLocationBlockMap();
        std::map<std::string, locationBlock>::iterator it = tmp.begin();
        for (; it != tmp.end(); it++)
            std::cout << it->second << std::endl;
    }

    return out;
}