/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lagea <lagea@student.s19.be>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 18:32:27 by lagea             #+#    #+#             */
/*   Updated: 2024/12/16 19:11:38 by lagea            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __TOKENIZER_HPP__
# define __TOKENIZER_HPP__

#include <iostream>
#include <vector>

enum e_tokenType
{
    keyword,
    value,
    openbracket,
    closebracket,
    delimiter
};

typedef struct s_token
{
    int type;
    std::string value;
}t_token;

class tokenizer
{
    public:
        tokenizer(std::string);
        ~tokenizer();
        
        std::vector<t_token> getTokensVector() const;
        
    private:
        std::vector<t_token> _tokensVector;
};

#endif