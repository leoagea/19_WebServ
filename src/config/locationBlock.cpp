/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   locationBlock.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lagea <lagea@student.s19.be>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 17:05:04 by lagea             #+#    #+#             */
/*   Updated: 2024/12/23 19:27:48 by lagea            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "locationBlock.hpp"

locationBlock::locationBlock(std::vector<t_token> &vec) : _tokenVec(vec), _uri(""), _root(""), _index(""), _autoindex(false), _iscgi(false),_cgi(false, ""), _cgipath(""), _allowedmethods(0)
{
    std::cout << "Test" << std::endl;
}

locationBlock::~locationBlock()
{
    
}

//True if location block is about cgi
bool locationBlock::getCgi() const
{
    return _iscgi;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void locationBlock::parseAllLocationVariables()
{
    if (_tokenVec[0].type == keyword && _tokenVec[0].value == "location"){
        if (_tokenVec[2].type != openbracket){
            std::cerr << "Error: config file: expected: location path {}" << std::endl;
        }
        else{
            if (isCGI(_tokenVec[1].value))
                _iscgi = true;
        }
    }
    
    int i = 3;
    for(i; i < (int)_tokenVec.size(); i++){
        t_token current = _tokenVec[i];
        
    }
}

bool locationBlock::isCGI(std::string &path)
{
    if (!PathChecking::isAbsolutePath(path)){
        if (path != ".php$"){
            std::cerr << "Error: config file: cgi not supported, expected only php" << std::endl;
            return false;
        }
    }
    else
        return false;
    return true;
}