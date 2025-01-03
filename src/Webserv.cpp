/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserv.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lagea <lagea@student.s19.be>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/03 14:59:09 by lagea             #+#    #+#             */
/*   Updated: 2025/01/03 15:15:59 by lagea            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Webserv.hpp"

Webserv::Webserv(const std::string &configPath) : _config(configPath)
{
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