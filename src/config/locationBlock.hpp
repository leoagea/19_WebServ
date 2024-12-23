/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   locationBlock.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lagea <lagea@student.s19.be>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 17:03:16 by lagea             #+#    #+#             */
/*   Updated: 2024/12/23 19:27:07 by lagea            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __LOCATIONBLOCK_HPP__
# define __LOCATIONBLOCK_HPP__

#include <iostream>
#include <vector>
#include <map>

#include "tokenizer.hpp"
#include "PathChecking.hpp"

enum e_allowed_methods{
    GET = 1 << 0,
    POST = 1 << 1,
    DELETE = 1 << 2,
    UPLOAD = 1 << 3
};

typedef struct s_redirect
{
    int htppcode;
    std::string newpath;
}t_redirect;


class locationBlock
{
    public:
        locationBlock(std::vector<t_token> &);
        ~locationBlock();
        
        bool getCgi() const;
        
    private:
        std::vector<t_token> _tokenVec;
        
        std::string _uri;
        std::string _root;
        std::string _index;
        bool _autoindex;
        bool _iscgi;
        std::pair<bool, std::string> _cgi; //used to store token nam,e of cgi path, and if token has been read
        std::string _cgipath;
        unsigned char _allowedmethods;

        void parseAllLocationVariables();

        bool isCGI(std::string &);
};

#endif