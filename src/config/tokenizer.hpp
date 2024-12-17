/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lagea <lagea@student.s19.be>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 18:32:27 by lagea             #+#    #+#             */
/*   Updated: 2024/12/17 20:11:24 by lagea            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __TOKENIZER_HPP__
# define __TOKENIZER_HPP__

#include <iostream>
#include <vector>

#include "../../inc/Color.h"

enum e_tokenType
{
    keyword,
    openbracket,
    closebracket,
    semicolon,
    number,
    string
};

typedef struct s_token
{
    e_tokenType type;
    std::string value;
}t_token;

class Tokenizer
{
    public:
        Tokenizer(std::string &);
        ~Tokenizer();
        
        std::vector<t_token> getTokensVector() const;
        
    private:
        std::vector<t_token> _tokensVector;

        void splitToken(std::string &);
        bool isKeyword(std::string &);
        void verifTokenizerStringEmpty(std::string &);
};

std::ostream &operator<<(std::ostream &, const Tokenizer &);

#endif