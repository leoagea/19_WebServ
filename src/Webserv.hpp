/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lagea <lagea@student.s19.be>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/03 14:53:34 by lagea             #+#    #+#             */
/*   Updated: 2025/01/03 15:15:14 by lagea            ###   ########.fr       */
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
        
        void initialiseLogSystem();
        void initialiseErrorPageGenerator();

    private:
        ConfigFile _config;
        Log _log;
        LogReporter _reportLog;
        ErrorPageGnerator _generator;
        
};

#endif