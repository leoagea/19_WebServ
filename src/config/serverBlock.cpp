/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   serverBlock.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lagea <lagea@student.s19.be>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 13:28:47 by lagea             #+#    #+#             */
/*   Updated: 2024/12/17 22:27:15 by lagea            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "serverBlock.hpp"

ServerBlock::ServerBlock(std::vector<t_token> &tokenVec, int *j) : _servername(""), _rootdir(""), _index(""), _acceslogdir(""), _errorlogdir(""), _bodysizelimit(-1) 
{
    parseAllServerVariables(tokenVec, j);
}

ServerBlock::~ServerBlock()
{
    
}

void ServerBlock::parseAllServerVariables(std::vector<t_token> &tokenVec, int *j)
{
    int i = 0;
    for (i = *j; i < (int)tokenVec.size() && tokenVec[i].type != closebracket; i++){
        t_token token = tokenVec[i];
        if (token.type == keyword && token.value == "listen" && tokenVec[i + 2].type == semicolon){
            token = tokenVec[i + 1];
            if (token.type == number){
                parseListeningPort(token.value);
            }    
        }
        else if (token.type == keyword && token.value == "server_name" && tokenVec[i + 2].type == semicolon){
            parseServerName(tokenVec[++i]);
        }
        else if (token.type == keyword && token.value == "root" && tokenVec[i + 2].type == semicolon){
            parseServerName(tokenVec[++i]);
        }
    }
    
    if(i == 0)
        *j += 1;
    *j = i;
}

std::vector<int> ServerBlock::getListeningPortsVector() const
{
    return _listeningports;
}

std::string ServerBlock::getServerName() const
{
    return _servername;
}

std::string ServerBlock::getRootDir() const
{
    return _rootdir;
}

void ServerBlock::parseListeningPort(std::string &num)
{
    int port = atoi(num.c_str());
    if (port <= 1023 || port > UINT16_MAX){
        std::cerr << "Error: config file: port range exceeded 1024 - 65535" << std::endl;
    }
    else{
        if (!_listeningports.empty()){
            if (find(_listeningports.begin(), _listeningports.end(), port) != _listeningports.end())
                _listeningports.push_back(port);
            else
                std::cerr << "Error: config file: port already used" << std::endl;
        }
        else
            _listeningports.push_back(port);
    }
}

void ServerBlock::parseServerName(t_token &token)
{
    if (token.type == string)
        _servername = token.value;
}

void ServerBlock::parseRootDir(t_token &token)
{
    std::cout << "Test    " << token.value << std::endl;
    if (token.type == string){
        if (PathChecking::exist(token.value) && PathChecking::isDirectory(token.value))
            _rootdir = token.value;
    }
}

std::ostream &operator<<(std::ostream &out, const ServerBlock &obj)
{
    out << BLUE << "Server Block" << RESET << std::endl;
    
    out << CYAN << "Listening Port" << RESET << std::endl;
    std::vector<int>::iterator it;
    std::vector<int> vec = obj.getListeningPortsVector();
    int i;
    for (i = 0, it = vec.begin(); it != vec.end(); i++,  it++){
        out << "Port "<< i << ":  " << *it << std::endl;
    }
    out << CYAN << "Server Name" << RESET << std::endl;
    out << "Name:  " << obj.getServerName() << std::endl;

    out << CYAN << "Root Dir" << RESET << std::endl;
    std::string path = obj.getRootDir();
    out << "Dir:  " << path << std::endl;
    out << "Exist: " << PathChecking::exist(path) << "  IsDir:  " << PathChecking::isDirectory(path)
        << "  R:  " << PathChecking::getReadPermission(path) << "  R:  " << PathChecking::getWritePermission(path) << "  R:  " << PathChecking::getExecPermission(path) << std::endl; 
    return out;
}