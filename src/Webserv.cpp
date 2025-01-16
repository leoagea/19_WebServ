/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserv.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lagea <lagea@student.s19.be>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/03 14:59:09 by lagea             #+#    #+#             */
/*   Updated: 2025/01/15 15:47:39 by lagea            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Webserv.hpp"

Webserv::Webserv(const std::string &configPath) : _config(), _log(), _reportLog(), _generator()
{
    ConfigFile config(configPath);
    _config = config;
}

Webserv::~Webserv()
{
}

ConfigFile Webserv::getConfigFileObject() const
{
    return _config;
}

LogReporter Webserv::getLogReporterObject() const
{
    return _reportLog;
}

ErrorPageGnerator Webserv::getErrorPageGenObject() const
{
    return _generator;
}

void Webserv::initialiseConfig()
{
    try
    {
        _config.loadConfFile(); 
    }
    catch(const std::exception& e)
    {
        throw std::runtime_error(e.what());
    }
}

void Webserv::initialiseLogSystem()
{
    Log log(&_config, 0);
    LogReporter report(&log);

    _reportLog = report;
}

void Webserv::initialiseErrorPageGenerator()
{
    ServerBlock server = _config.getServerBlockByIndex(0);
    ErrorPageGnerator generator(server);

    _generator = generator;
}