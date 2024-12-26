/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   configFileParser.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lagea <lagea@student.s19.be>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 13:20:44 by lagea             #+#    #+#             */
/*   Updated: 2024/12/26 13:56:29 by lagea            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "configFileParser.hpp"

ConfigFile::ConfigFile(std::string path) : _tokenizerString("")
{
    if (!isConfPathValid(path)){
        std::cerr << "Error: config file: wrong extension, expected .conf" << std::endl;
        exit(EXIT_FAILURE);
    }

    
    std::fstream file(path.c_str());
    
    if (!file.is_open()){
        std::cerr << "Error: config file: failed to open" << std::endl;
        exit(EXIT_FAILURE);
    }

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

    if (_tokenizerString.empty()){
        std::cerr << "Error: config file: empty file" << std::endl;
        exit(EXIT_FAILURE);
    }

    // std::cout << _tokenizerString << std::endl;
    
    Tokenizer token(_tokenizerString);
    _tokensVec = token.getTokensVector();

    //Print all tokens
    // std::cout << token << std::endl;
    
    splitServerBlock();
}

ConfigFile::~ConfigFile()
{
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
    for(int i = 0; i < (int)_tokensVec.size(); i++){
        if(_tokensVec[i].type == keyword && _tokensVec[i].value == "server"){
            if(_tokensVec[i + 1].type == openbracket){
                i += 2;
                ServerBlock server(_tokensVec, &i);
                // std::cout << server << std::endl;
                _serverlist.push_back(server);
            }
                
        }
    }
}