/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   configFileParser.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lagea <lagea@student.s19.be>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 13:20:44 by lagea             #+#    #+#             */
/*   Updated: 2024/12/16 19:12:26 by lagea            ###   ########.fr       */
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
    }

    if(_tokenizerString.empty()){
        std::cerr << "Error: config file: empty file" << std::endl;
        return ;
    }

    std::cout << _tokenizerString << std::endl;
    
    tokenizer token(_tokenizerString);
    _tokensVec = token.getTokensVector();

}

ConfigFile::~ConfigFile()
{
}
