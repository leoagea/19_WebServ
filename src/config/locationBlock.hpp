/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   locationBlock.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmailleu <kmailleu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 17:03:16 by lagea             #+#    #+#             */
/*   Updated: 2025/05/06 19:10:49 by kmailleu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#ifndef __LOCATIONBLOCK_HPP__
#define __LOCATIONBLOCK_HPP__

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
    locationBlock &operator=(const locationBlock &other);

    bool getCgi() const;
    std::string getUri() const;
    std::string getRootDirLoc() const;
    std::string getIndexLoc() const;
    std::string getRootIndexConcatenate() const;
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
    std::string _rootIndex;
    int _autoindex;
    bool _isredirect; // to indicate if location redirect to another location
    std::pair<int, std::string> _redirect;
    bool _iscgi;
    std::pair<int, std::string> _cgi; // used to store token nam,e of cgi path, and if token has been read
    std::string _cgipath;
    int _allowedget;
    int _allowedpost;
    int _alloweddelete;
    int _allowedupload;
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