/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserv.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lagea <lagea@student.s19.be>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/03 14:59:09 by lagea             #+#    #+#             */
/*   Updated: 2025/01/16 15:36:17 by lagea            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Webserv.hpp"

Webserv::Webserv(const std::string &configPath, char **env)
{
    ConfigFile config(configPath);
    _config = config;
    initialiseEnv(env);
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

std::map<std::string, std::string> Webserv::getEnvMap() const
{
    return _env;
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

void Webserv::initialiseEnv(char **env)
{
    int i = -1;
    
    while (env[++i]){
        std::stringstream ss(env[i]);
        std::string key;
        std::string val;
        getline(ss, key, '=');
        getline(ss, val);
        _env[key] = val;
    }
}