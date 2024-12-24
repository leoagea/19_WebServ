/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   locationBlock.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lagea <lagea@student.s19.be>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 17:03:16 by lagea             #+#    #+#             */
/*   Updated: 2024/12/24 17:04:49 by lagea            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __LOCATIONBLOCK_HPP__
# define __LOCATIONBLOCK_HPP__

#include <iostream>
#include <vector>
#include <map>

#include "tokenizer.hpp"
#include "PathChecking.hpp"
#include "serverBlock.hpp"

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

class ServerBlock;

class locationBlock
{
    public:
        locationBlock(ServerBlock &, std::vector<t_token> &);
        ~locationBlock();
        
        bool getCgi() const;
        std::string getUri() const;
        std::string getRootDirLoc() const;
        std::string getIndexLoc() const;
        bool getAutoIndexLoc() const;
        bool getAllowedMethodGET() const;
        bool getAllowedMethodPOST() const;
        bool getAllowedMethodDELETE() const;
        bool getAllowedMethodUPLOAD() const;
        std::string getCgiScriptName() const;
        std::string getCgiPath() const;
        bool getIsredirect() const;
        int getRedirectReturnStatus() const;
        std::string getRedirectName() const;
        
    private:
        ServerBlock &_server;
        std::vector<t_token> _tokenVec;
        
        std::string _uri;
        std::string _root;
        std::string _index;
        bool _autoindex;
        bool _isredirect; //to indicate if location redirect to another location
        std::pair<int, std::string> _redirect;
        bool _iscgi;
        std::pair<bool, std::string> _cgi; //used to store token nam,e of cgi path, and if token has been read
        std::string _cgipath;
        unsigned char _allowedmethods;

        void parseAllLocationVariables();
        void parseRootDir(t_token &);
        void parseIndex(t_token &);
        void parseAutoIndex(t_token &);
        void parseAllowedMethod(t_token &);
        void parseInclude(t_token &);
        void parseCgiScriptName(t_token &);
        void parseRedirect(t_token &, t_token &);
        
        bool isCGI(std::string &);
};

std::ostream &operator<<(std::ostream &, const locationBlock &);

#endif