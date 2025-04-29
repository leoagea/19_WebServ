/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   serverBlock.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lagea <lagea@student.s19.be>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 13:28:47 by lagea             #+#    #+#             */
/*   Updated: 2025/04/29 13:43:05 by lagea            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#ifndef __SERVERBLOCK_HPP__
#define __SERVERBLOCK_HPP__

#define UINT16_MAX 65535

#include <vector>
#include <iostream>
#include <map>
#include <set>
#include <algorithm>
#include <fstream>
#include <limits>
#include <sstream>
#include "fcntl.h"

#include "ErrorReporter.hpp"
#include "locationBlock.hpp"
#include "tokenizer.hpp"
#include "PathChecking.hpp"
#include "../../inc/Color.h"

class locationBlock;

class ServerBlock
{
public:
    ServerBlock();
    ServerBlock(int, std::vector<t_token> &, int *, const ErrorReporter &);
    ~ServerBlock();

    int getListeningPort() const;
    std::string getServerName() const;
    std::string getRootDir() const;
    std::string getIndex() const;
    int getBodySizeLimit() const;
    std::string getErrorPagePath(int) const;
    std::map<int, std::string> getErrorPagesMap() const;
    std::string getHost() const;
    std::vector<int> getHostBytesVector() const;
    int getHostBytesByIndex(int) const;
    std::map<std::string, locationBlock> getLocationBlockMap() const;
    locationBlock getLocationBlockByString(std::string &) const;

private:
    int _listeningports;
    std::string _servername;
    std::string _rootdir;
    std::string _index;
    int _bodysizelimit;
    std::string _host;
    std::vector<int> _hostbytes;
    std::map<std::string, class locationBlock> _locationblock;
    std::map<int, std::string> _errorpages;
    ErrorReporter _reportError;

    void initializeMapErrorPages();
    void parseAllServerVariables(int, std::vector<t_token> &, int *);
    void parseListeningPort(t_token &);
    void parseServerName(t_token &);
    void parseRootDir(t_token &);
    void parseIndex(t_token &);
    void parseLimitBodySize(t_token &);
    void parseErrorsPages(t_token &, t_token &);
    void parseHost(t_token &);

    bool isHostValid(std::string &);
    bool isLogExtensionValid(std::string &);

    std::vector<std::string> checkAllDefined();
};

std::ostream &operator<<(std::ostream &, const ServerBlock &);

#endif