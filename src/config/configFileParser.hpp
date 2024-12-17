/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   configFileParser.hpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lagea <lagea@student.s19.be>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 13:20:44 by lagea             #+#    #+#             */
/*   Updated: 2024/12/17 18:09:58 by lagea            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __CONFIGPARSERFILE_HPP__
# define __CONFIGPARSERFILE_HPP__

#include <iostream>
#include <vector>
#include <fstream>

#include "serverBlock.hpp"
#include "tokenizer.hpp"

class ConfigFile
{
    public:
        ConfigFile(std::string);    
        ~ConfigFile();

    private:
        std::vector<std::string> _configFileVector; //Vector containing conf file used to write errors properly on terminal
        std::string _tokenizerString;
        std::vector<t_token> _tokensVec;
        std::vector<class serverBlock> _serverlist;
};

#endif