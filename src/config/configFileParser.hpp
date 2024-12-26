/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   configFileParser.hpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lagea <lagea@student.s19.be>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 13:20:44 by lagea             #+#    #+#             */
/*   Updated: 2024/12/26 13:59:38 by lagea            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __CONFIGPARSERFILE_HPP__
# define __CONFIGPARSERFILE_HPP__

#include <iostream>
#include <vector>
#include <fstream>

#include "serverBlock.hpp"
#include "tokenizer.hpp"

#define DEFAULT_PATH "config/config.conf"

class ConfigFile
{
    public:
        ConfigFile(std::string);    
        ~ConfigFile();
        
        std::vector<ServerBlock> getServerBlockVector() const;
        ServerBlock getServerBlockByIndex(int) const;

    private:
        std::vector<std::string> _configFileVector; //Vector containing conf file used to write errors properly on terminal
        std::string _tokenizerString;
        std::vector<t_token> _tokensVec;
        std::vector<ServerBlock> _serverlist;

        bool isConfPathValid(std::string &);
        void splitServerBlock();
        
};

#endif