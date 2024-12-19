/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   serverBlock.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lagea <lagea@student.s19.be>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 13:28:47 by lagea             #+#    #+#             */
/*   Updated: 2024/12/19 19:25:14 by lagea            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "serverBlock.hpp"

ServerBlock::ServerBlock(std::vector<t_token> &tokenVec, int *j) : _servername({"webserv"}), _rootdir(""), _index(""), _acceslogdpath(""), _errorlogpath(""), _bodysizelimit(-1) 
{
    initializeMapErrorPages();
    parseAllServerVariables(tokenVec, j);
}

ServerBlock::~ServerBlock()
{
    
}

//Return -1 if index is out of bound in the vector
int ServerBlock::getListeningPortByIndex(int indexOfVector) const
{
    try{
        _listeningports.at(indexOfVector);
    }
    catch (std::exception &e){
        return -1;
    }
    return _listeningports.at(indexOfVector);
}

std::vector<int> ServerBlock::getListeningPortsVector() const
{
    return _listeningports;
}

//Rerturn empty string if index is out of bound
std::string ServerBlock::getServerNameByIndex(int indexOfVector) const
{
    try{
        _servername.at(indexOfVector);
    }
    catch (std::exception &e){
        return "";
    }
    return _servername.at(indexOfVector);
}

std::vector<std::string> ServerBlock::getServerNameVector() const
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

std::string ServerBlock::getAccesLogFilePath() const
{
    return _acceslogdpath;
}

//Throw a runtime_error if file failed to be opened
std::ofstream *ServerBlock::getAccessLogStream() const
{
    std::ofstream *file = new std::ofstream;
    std::string path = _acceslogdpath;
    if ((PathChecking::exist(path)) && PathChecking::isFile(path) && PathChecking::getWritePermission(path)){
        file->open(path.c_str());
        if (!file->is_open()){
            delete file;
            throw std::runtime_error("Error: failed to open access log file");
        }
    }
    return file;
}

std::string ServerBlock::getErrorsLogFilePath() const
{
    return _errorlogpath;
}

//Throw a runtime_error if file failed to be opened
std::ofstream *ServerBlock::getErrorsLogStream() const
{
    std::ofstream *file = new std::ofstream;
    std::string path = _errorlogpath;
    if ((PathChecking::exist(path)) && PathChecking::isFile(path) && PathChecking::getWritePermission(path)){
        file->open(path.c_str());
        if (!file->is_open()){
            delete file;
            throw std::runtime_error("Error: failed to open access log file");
        }
    }
    return file;
}

int ServerBlock::getBodySizeLimit() const
{
    return _bodysizelimit;
}

//Return path to file if redefined in conf file, return empty string otherwise
//If error number is not handle, return "nulll"
std::string ServerBlock::getErrorPagePath(int error) const
{
    std::map<int, std::string>::const_iterator it = _errorpages.find(error);
    
    if (it != _errorpages.end()){
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

///////////////////////////////////////////////////////////////////////////////////////////////////

void ServerBlock::initializeMapErrorPages()
{
    _errorpages[400] = ""; //Bad request
    _errorpages[403] = ""; //Forbidden
    _errorpages[404] = ""; //Not found
    _errorpages[405] = ""; //Method not allowed
    _errorpages[408] = ""; //Request timeout
    _errorpages[413] = ""; //Payload too large
    _errorpages[500] = ""; //Internal server error
}

void ServerBlock::parseAllServerVariables(std::vector<t_token> &tokenVec, int *j)
{
    int i = 0;
    for (i = *j; i < (int)tokenVec.size() && tokenVec[i].type != closebracket; i++){
        t_token token = tokenVec[i];
        if (token.type == keyword && token.value == "listen"){
            i++;
            while(tokenVec[i].type != semicolon){
                if (tokenVec[i].type == keyword)
                    std::cerr << "Error: config file: expected ; before new line" << std::endl;
                else
                    parseListeningPort(tokenVec[i]);
                i++;
            }
        }
        else if (token.type == keyword && token.value == "server_name"){
            i++;
            while(tokenVec[i].type != semicolon){
                if (tokenVec[i].type == keyword)
                    std::cerr << "Error: config file: expected ; before new line" << std::endl;
                else
                    parseServerName(tokenVec[i]);
                i++;
            }
        }
        else if (token.type == keyword && token.value == "root" && tokenVec[i + 2].type == semicolon){
            parseRootDir(tokenVec[++i]);
        }
        else if (token.type == keyword && token.value == "index" && tokenVec[i + 2].type == semicolon){
            parseIndex(tokenVec[++i]);
        }
        else if (token.type == keyword && token.value == "access_log" && tokenVec[i + 2].type == semicolon){
            parseAccesLogPath(tokenVec[++i]);
        }
        else if (token.type == keyword && token.value == "errors_log" && tokenVec[i + 2].type == semicolon){
            parseErrorsLogPath(tokenVec[++i]);
        }
        else if (token.type == keyword && token.value == "limit_body_size" && tokenVec[i + 2].type == semicolon){
            parseLimitBodySize(tokenVec[++i]);
        }
        else if (token.type == keyword && token.value == "error_page" && tokenVec[i + 3].type == semicolon){
            parseErrorsPages(tokenVec[i + 1], tokenVec[i + 2]);
            i += 2;
        }
    }

    if(i == 0)
        *j += 1;
    *j = i;
}

void ServerBlock::parseListeningPort(t_token &token)
{
    if (token.type == number)
    {
        int port = atoi(token.value.c_str());
        if (port <= 1023 || port > UINT16_MAX){
            std::cerr << "Error: config file: port range exceeded 1024 - 65535" << std::endl;
        }
        else{
            if (!_listeningports.empty()){
                if (find(_listeningports.begin(), _listeningports.end(), port) == _listeningports.end())
                    _listeningports.push_back(port);
                else
                    std::cerr << "Error: config file: port already used" << std::endl;
            }
            else
                _listeningports.push_back(port);
        }
    }
}

void ServerBlock::parseServerName(t_token &token)
{
    if (token.type == string)
    {
        if (!_servername.empty()){
            if (find(_servername.begin(), _servername.end(), token.value) == _servername.end())
                _servername.push_back(token.value);
            else
                std::cerr << "Error: config file: port already used" << std::endl;
        }
        else
            _servername.push_back(token.value);
    }
}

void ServerBlock::parseRootDir(t_token &token)
{
    if (token.type == string){
        if (PathChecking::isAbsolutePath(token.value) && PathChecking::exist(token.value) && PathChecking::isDirectory(token.value))
            _rootdir = token.value;
    }
}

void ServerBlock::parseIndex(t_token &token)
{
    if (token.type == string){
        std::string path;
        
        if (_rootdir[_rootdir.size() - 1] == '/')
            path = _rootdir + token.value;
        else
            path = _rootdir + "/" + token.value;

        if (PathChecking::isAbsolutePath(path) && PathChecking::exist(path) && PathChecking::isFile(path) && PathChecking::getReadPermission(path))
            _index = path;
    }
}

void ServerBlock::parseAccesLogPath(t_token &token)
{
    if (token.type == string){
        std::string path =token.value;
        
        if(!PathChecking::isAbsolutePath(token.value)){
            if (_rootdir[_rootdir.size() - 1] == '/')
                path = _rootdir + token.value;
            else
                path = _rootdir + "/" + token.value;
        }
            
        if (!PathChecking::exist(path)){
            int fd = open(path.c_str(), O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
            if (fd  == -1)
                std::cerr << "Error: config file: failed to create access log file" << std::endl;
            else
                close(fd);
        }
        _acceslogdpath = path;
    }
}

void ServerBlock::parseErrorsLogPath(t_token &token)
{
    if (token.type == string){
        std::string path = token.value;
        
        if(!PathChecking::isAbsolutePath(token.value)){
            if (_rootdir[_rootdir.size() - 1] == '/')
                path = _rootdir + token.value;
            else
                path = _rootdir + "/" + token.value;
        }
        
        if (!PathChecking::exist(path)){
            int fd = open(path.c_str(), O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
            if (fd  == -1)
                std::cerr << "Error: config file: failed to create error log file" << std::endl;
            else
                close(fd);
        }
        _errorlogpath = path;
    }
}

void ServerBlock::parseLimitBodySize(t_token &token)
{
    if (token.type == number){
        int maxbodysize = atoi(token.value.c_str());
        if (maxbodysize <= 0 || maxbodysize >= 1025)
            std::cerr << "Error: config file: max body size range exceeded 1 - 1024" << std::endl;
        else
            _bodysizelimit = maxbodysize;
    }
    else
        std::cerr << "Error: config file: expected a number" << std::endl;
}

void ServerBlock::parseErrorsPages(t_token &num, t_token &token)
{
    if (num.type == number && token.type == string){
        int numpage = atoi(num.value.c_str());
        if (_errorpages.find(numpage) != _errorpages.end()){
            std::string path = token.value;
        
            if(!PathChecking::isAbsolutePath(token.value)){
                if (_rootdir[_rootdir.size() - 1] == '/')
                    path = _rootdir + token.value;
                else
                    path = _rootdir + "/" + token.value;
            }
            
            if (PathChecking::exist(path) && PathChecking::isFile(path) && PathChecking::getReadPermission(path)){
                _errorpages[numpage] = path;
            }
        }
    }
    else
        std::cerr << "Error: config file: expected number and a path" << std::endl;
}

//Print every private attributes for debugging purpose
std::ostream &operator<<(std::ostream &out, const ServerBlock &obj)
{
    out << BLUE << "Server Block" << RESET << std::endl;
    
    out << CYAN << "Listening Port" << RESET << std::endl;
    {
        std::vector<int>::iterator it;
        std::vector<int> vec = obj.getListeningPortsVector();
        int i;
        for (i = 0, it = vec.begin(); it != vec.end(); i++,  it++){
            out << "Port "<< i << ":  " << *it << std::endl;
        }
    }
    std::cout << "Test: getListeningPortByIndex: index = -1, index = 0" << std::endl;
    std::cout << "index -1:  " << obj.getListeningPortByIndex(-1) << "   index 0:  " << obj.getListeningPortByIndex(0) << std::endl;
    
    out << CYAN << "Server Name" << RESET << std::endl;
    {
        std::vector<std::string>::iterator it;
        std::vector<std::string> vec = obj.getServerNameVector();
        int i;
        for (i = 0, it = vec.begin(); it != vec.end(); i++,  it++){
            out << "Port "<< i << ":  " << *it << std::endl;
        }
    }
    std::cout << "Test: getServerNametByIndex: index = -1, index = 0" << std::endl;
    std::cout << "index -1:  " << obj.getServerNameByIndex(-1) << "   index 0:  " << obj.getServerNameByIndex(0) << std::endl;

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
    
    out << CYAN << "Acces log" << RESET << std::endl;
    std::string pathAccesLog = obj.getAccesLogFilePath();
    out << "Dir:  " << pathAccesLog << std::endl;
    try{
        std::ofstream *logFile = obj.getAccessLogStream();
        out << "Is open:  " << logFile->is_open() << "\n"
            << "Test writing acces log: abc" << std::endl;
        *logFile << "abc\n";
        logFile->close();
    }
    catch (std::exception &e){
        std::cerr << e.what() << std::endl;
    }

    out << CYAN << "Errors log" << RESET << std::endl;
    std::string pathErrorsLog = obj.getErrorsLogFilePath();
    out << "Dir:  " << pathErrorsLog << std::endl;
    try{
        std::ofstream *logFile = obj.getErrorsLogStream();
        out << "Is open:  " << logFile->is_open() << "\n"
            << "Test writing acces log: abc" << std::endl;
        *logFile << "abc\n";
        logFile->close();
    }
    catch (std::exception &e){
        std::cerr << e.what() << std::endl;
    }

    out << CYAN << "Body Size Limit" << RESET << std::endl;
    out << "Limit: " << obj.getBodySizeLimit() << std::endl;
    
    out << CYAN << "Error pages" << RESET << std::endl;
    std::map<int, std::string> map = obj.getErrorPagesMap();
    std::cout << "Iterate through map: " << std::endl;
    for(std::map<int, std::string>::iterator it = map.begin(); it != map.end(); it++){
        std::cout << "  Error:  " << it->first << "   Path:  '" << it->second << "'" << std::endl;
    }
    std::cout << "Get page 400:  '" << obj.getErrorPagePath(400) << "'" << std::endl;
    std::cout << "Get page 404:  '" << obj.getErrorPagePath(404) << "'" << std::endl;
    std::cout << "Get page 1000:  '" << obj.getErrorPagePath(1000) << "'" << std::endl;

    return out;
}