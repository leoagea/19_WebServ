/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   configFileParser.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lagea <lagea@student.s19.be>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 13:20:44 by lagea             #+#    #+#             */
/*   Updated: 2024/12/17 18:16:04 by lagea            ###   ########.fr       */
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

    // std::vector<t_token>::iterator it = _tokensVec.begin();
    // for(; it != _tokensVec.end(); it++){
    //     std::cout << "type: " << it->type << "   value: " << it->value << std::endl;
    // }

}

ConfigFile::~ConfigFile()
{
}
