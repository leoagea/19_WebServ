/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lagea <lagea@student.s19.be>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 18:35:08 by lagea             #+#    #+#             */
/*   Updated: 2024/12/24 16:19:19 by lagea            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tokenizer.hpp"

Tokenizer::Tokenizer(std::string &configContent)
{
    splitToken(configContent);
}

Tokenizer::~Tokenizer()
{
    
}

std::vector<t_token> Tokenizer::getTokensVector() const
{
    return _tokensVector;
}

void Tokenizer::splitToken(std::string &configContent)
{
    for(int i=0; i < (int)configContent.size(); i++){
        std::string::iterator it;
        std::string::iterator it2;
        t_token token;
        
        if(configContent[i] && isspace(configContent[i]))
            continue;
        else if(configContent[i] && configContent[i] == ';'){
            token.type = semicolon;
            token.value = ";";
            _tokensVector.push_back(token);
            it = configContent.begin() + i;
            configContent.erase(it);
            i -= 1;
        }
        else if(configContent[i] && configContent[i] == '{'){
            token.type = openbracket;
            token.value = "{";
            _tokensVector.push_back(token);
            it = configContent.begin() + i;
            configContent.erase(it);
        }
        else if(configContent[i] && configContent[i] == '}'){
            token.type = closebracket;
            token.value = "}";
            _tokensVector.push_back(token);
            it = configContent.begin() + i;
            configContent.erase(it);
            i -= 1;
        }
        else if(configContent[i] && (isdigit(configContent[i]) || configContent[i] == '-')){
            int start = i;
            it = configContent.begin() + i;
            if(configContent[i] == '-')
                i++;
            while(configContent[i] && isdigit(configContent[i])){
                if (configContent[i + 1] == '.')
                    i += 2;
                else
                    i++;
            }
            it2 = configContent.begin() + i;
            
            token.value = configContent.substr(start, i - start);
            token.type = number;
            _tokensVector.push_back(token);
            configContent.erase(it, it2);
            i = start - 1;
        }
        else{
            int start = i;
            it = configContent.begin() + i;
            while(configContent[i] && configContent[i] != ' ' && configContent[i] != ';'){
                i++;
            }
            it2 = configContent.begin() + i;
            
            token.value = configContent.substr(start, i - start);
            if(isKeyword(token.value))
                token.type = keyword;
            else
                token.type = string;
            _tokensVector.push_back(token);
            configContent.erase(it, it2);
            i = start - 1;
        }

    }
    verifTokenizerStringEmpty(configContent);
}

bool Tokenizer::isKeyword(std::string &value)
{
    if(value == "listen" || value == "server_name" || value == "root" || value == "index" \
        || value == "access_log" || value == "errors_log" || value == "location" \
        || value == "autoindex" || value == "limit_body_size" || value == "server" \
        || value == "error_page" || value == "host" || value == "allowed_methods" \
        || value == "include" || value == "cgi_param" || value == "return")
        return true;
    return false;
}

void Tokenizer::verifTokenizerStringEmpty(std::string &tokenString)
{
    for(int i=0; i < (int)tokenString.size(); i++)
        if(tokenString[i] != ' ')
            std::cerr << "Error: config file: dunno" << std::endl;
}

std::ostream &operator<<(std::ostream &out, const Tokenizer &obj)
{
    std::vector<t_token> vec = obj.getTokensVector();
    
    std::vector<t_token>::iterator it;
    out << BLUE << "List of token:" << RESET << std::endl;
    for (it = vec.begin(); it != vec.end(); it++){
        out << "type: " << it->type << "   value: " << it->value << std::endl;
    }
    return out;
}