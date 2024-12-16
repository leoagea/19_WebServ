/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lagea <lagea@student.s19.be>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 18:35:08 by lagea             #+#    #+#             */
/*   Updated: 2024/12/16 19:10:26 by lagea            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tokenizer.hpp"

tokenizer::tokenizer(std::string configContent)
{
    for(int i=0; i < (int)configContent.size(); i++){
        t_token token;
        if(isspace(configContent[i]))
            continue;
        else if(configContent[i] == '{'){
            token.type = openbracket;
            token.value = "{";
            _tokensVector.push_back(token);
        }
        else if(configContent[i] == '}'){
            token.type = closebracket;
            token.value = "}";
            _tokensVector.push_back(token);
        }
    }
}

tokenizer::~tokenizer()
{
    
}

std::vector<t_token> tokenizer::getTokensVector() const
{
    return _tokensVector;
}
