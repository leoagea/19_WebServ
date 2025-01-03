/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lagea <lagea@student.s19.be>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/03 14:53:34 by lagea             #+#    #+#             */
/*   Updated: 2025/01/03 16:22:04 by lagea            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __WEBSERV_HPP__
# define __WEBSERV_HPP__

#include "config/configFileParser.hpp"
#include "log/Log.hpp"
#include "log/LogReporter.hpp"
#include "errors/ErrorPageGenerator.hpp"

class Webserv
{
    public:
        Webserv(const std::string &);
        ~Webserv();
        
        ConfigFile getConfigFileObject() const;
        LogReporter getLogReporterObject() const;
        ErrorPageGnerator getErrorPageGenObject() const;
        
        void initialiseConfig();
        void initialiseLogSystem();
        void initialiseErrorPageGenerator();

    private:
        ConfigFile _config;
        Log _log;
        LogReporter _reportLog;
        ErrorPageGnerator _generator;
        
};

#endif