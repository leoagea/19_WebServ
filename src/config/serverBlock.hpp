/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   serverBlock.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lagea <lagea@student.s19.be>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 13:28:47 by lagea             #+#    #+#             */
/*   Updated: 2024/12/17 22:19:00 by lagea            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __SERVERBLOCK_HPP__
# define __SERVERBLOCK_HPP__

#include <vector>
#include <iostream>
#include <map>
#include <set>
#include <limits>
#include <algorithm>

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
        std::vector<int> getListeningPortsVector() const;
        std::string getServerName() const;
        std::string getRootDir() const;
        
    private:
        std::vector<int> _listeningports; //index 0 is default port
        std::string _servername;
        std::string _rootdir;
        std::string _index;
        std::string _acceslogdir;
        std::string _errorlogdir;
        int _bodysizelimit;
        std::map<std::string ,class locationBlock> _locationblock;
        std::map<std::pair<int, std::string>, class locationBlock> _errorpages;

        void parseAllServerVariables(std::vector<t_token> &, int *);
        void parseListeningPort(std::string &);
        void parseServerName(t_token &);
        void parseRootDir(t_token &);
};

std::ostream &operator<<(std::ostream &, const ServerBlock &);

#endif