/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   configFileParser.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lagea <lagea@student.s19.be>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 13:20:44 by lagea             #+#    #+#             */
/*   Updated: 2024/12/26 17:49:49 by lagea            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "configFileParser.hpp"

ConfigFile::ConfigFile(std::string path) : _filepath(path) ,_tokenizerString("")
{
}

ConfigFile::~ConfigFile()
{
}

void ConfigFile::loadConfFile()
{
    if (!isConfPathValid(_filepath))
        throw std::runtime_error("Error: config file: wrong extension, expected .conf");
    
    std::fstream file(_filepath.c_str());
    
    if (!file.is_open())
        throw std::runtime_error("Error: config file: failed to open");

    std::string line;
    while (getline(file, line)){
        if (!line.size())
            continue;
            
        std::string::iterator it = std::find(line.begin(), line.end(), '#');
        if (it != line.end())
            line.erase(it, line.end());
            
        _tokenizerString += line;
        _configFileVector.push_back(line);
    }

    if (_tokenizerString.empty())
        throw std::runtime_error("Error: config file: empty file");

    parseConfFile();
}

void ConfigFile::reportError(int tokenIndex, const std::string &msg)
{
    _errors.insert(std::make_pair(tokenIndex, msg));
}

bool ConfigFile::isParsingFailed() const
{
    if (_errors.empty())
        return false;
    return true;
}

void ConfigFile::printErrors() const
{
    if (!_errors.empty()){
        std::map<int, std::string>::const_iterator it;

        for (it = _errors.begin(); it != _errors.end(); it++){
            std::cout << "Token " << it->first << " : " << it->second << std::endl;
        }
    }
}

std::vector<ServerBlock> ConfigFile::getServerBlockVector() const
{
    return _serverlist;
}

//Throw a out of range execption if index is out of bound of vector
//Otherwise return the ServerBlock object at index
ServerBlock ConfigFile::getServerBlockByIndex(int index) const
{
    return _serverlist.at(index);
}


///////////////////////////////////////////////////////////////////////////////////////////////////

void ConfigFile::parseConfFile()
{
    Tokenizer token(_tokenizerString);
    _tokensVec = token.getTokensVector();

    //Print all tokens
    // std::cout << token << std::endl;
    
    splitServerBlock();
}

bool ConfigFile::isConfPathValid(std::string &path)
{
    std::string extension = ".conf";
    std::string::iterator it = std::find_end(path.begin(), path.end(), extension.begin(), extension.end());
    
    if (it == path.end())
        return false;
    return true;
}

void ConfigFile::splitServerBlock()
{
    ErrorReporter reporter(this);
    
    for(int i = 0; i < (int)_tokensVec.size(); i++){
        if(_tokensVec[i].type == keyword && _tokensVec[i].value == "server"){
            if(_tokensVec[i + 1].type == openbracket){
                i += 2;
                ServerBlock server(_tokensVec, &i, reporter);
                // std::cout << server << std::endl;
                _serverlist.push_back(server);
            }
                
        }
    }
}