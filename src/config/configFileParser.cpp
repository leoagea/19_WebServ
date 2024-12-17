/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   configFileParser.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lagea <lagea@student.s19.be>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 13:20:44 by lagea             #+#    #+#             */
/*   Updated: 2024/12/17 22:24:44 by lagea            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "configFileParser.hpp"

ConfigFile::ConfigFile(std::string path) : _tokenizerString("")
{
    std::fstream file(path.c_str());
    
    if(!file.is_open()){
        std::cerr << "Error: config file: failed to open" << std::endl;
        return ;
    }

    std::string line;
    while(getline(file, line)){
        if(!line.size())
            continue;
        _tokenizerString += line;
        _configFileVector.push_back(line);
    }

    if(_tokenizerString.empty()){
        std::cerr << "Error: config file: empty file" << std::endl;
        return ;
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


void ConfigFile::splitServerBlock()
{
    for(int i = 0; i < (int)_tokensVec.size(); i++){
        if(_tokensVec[i].type == keyword && _tokensVec[i].value == "server"){
            if(_tokensVec[i + 1].type == openbracket){
                i += 2;
                ServerBlock server(_tokensVec, &i);
                std::cout << server << std::endl;
                _serverlist.push_back(server);
            }
                
        }
    }
}