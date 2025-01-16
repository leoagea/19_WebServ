/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   locationBlock.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lagea <lagea@student.s19.be>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 17:03:16 by lagea             #+#    #+#             */
/*   Updated: 2025/01/16 16:35:51 by lagea            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#ifndef __LOCATIONBLOCK_HPP__
# define __LOCATIONBLOCK_HPP__

#include <iostream>
#include <vector>
#include <map>

#include "ErrorReporter.hpp"
#include "tokenizer.hpp"
#include "PathChecking.hpp"
#include "serverBlock.hpp"
#include "../../inc/Struct.h"
#include "../../inc/Enum.h"

class ServerBlock;

class locationBlock
{
    public:
        locationBlock(ServerBlock &, std::vector<t_token> &, const ErrorReporter &);
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
        bool _allowedget;
        bool _allowedpost;
        bool _alloweddelete;
        bool _allowedupload;
        ErrorReporter _reportError;

        void parseAllLocationVariables();
        void parseRootDir(t_token &);
        void parseIndex(t_token &);
        void parseAutoIndex(t_token &);
        void parseAllowedMethod(t_token &);
        void parseInclude(t_token &);
        void parseCgiScriptName(t_token &);
        void parseRedirect(t_token &, t_token &);
        
        bool isCGI(std::string &, int);
};

std::ostream &operator<<(std::ostream &, const locationBlock &);

#endif