/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   serverBlock.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lagea <lagea@student.s19.be>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 13:28:47 by lagea             #+#    #+#             */
/*   Updated: 2024/12/23 15:34:16 by lagea            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __SERVERBLOCK_HPP__
# define __SERVERBLOCK_HPP__

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

#include "locationBlock.hpp"
#include "tokenizer.hpp"
#include "PathChecking.hpp"
#include "../../inc/Color.h"

class locationBlock;

class ServerBlock 
{
    public:
        ServerBlock(std::vector<t_token> &, int *);
        ~ServerBlock();

        ServerBlock getServerBlock() const;
        int getListeningPortByIndex(int) const;
        std::vector<int> getListeningPortsVector() const;
        std::string getServerNameByIndex(int) const;
        std::vector<std::string> getServerNameVector() const;
        std::string getRootDir() const;
        std::string getIndex() const;
        std::string getAccesLogFilePath() const;
        std::ofstream *getAccessLogStream() const;
        std::string getErrorsLogFilePath() const;
        std::ofstream *getErrorsLogStream() const;
        int getBodySizeLimit() const;
        std::string getErrorPagePath(int) const;
        std::map<int, std::string> getErrorPagesMap() const;
        std::string getHost() const;
        std::vector<int> getHostBytesVector() const;
        int getHostBytesByIndex(int) const;

    private:
        std::vector<int> _listeningports; //index 0 is default port
        std::vector<std::string> _servername;
        std::string _rootdir;
        std::string _index;
        std::string _acceslogdpath;
        std::string _errorlogpath;
        int _bodysizelimit;
        std::string _host;
        std::vector<int> _hostbytes;
        std::map<std::string ,class locationBlock> _locationblock;
        std::map<int, std::string> _errorpages;

        void initializeMapErrorPages();
        void parseAllServerVariables(std::vector<t_token> &, int *);
        void parseListeningPort(t_token &);
        void parseServerName(t_token &);
        void parseRootDir(t_token &);
        void parseIndex(t_token &);
        void parseAccesLogPath(t_token &);
        void parseErrorsLogPath(t_token &);
        void parseLimitBodySize(t_token &);
        void parseErrorsPages(t_token &, t_token &);
        void parseHost(t_token &);

        bool isHostValid(std::string &);
};

std::ostream &operator<<(std::ostream &, const ServerBlock &);

#endif