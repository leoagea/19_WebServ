/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserv.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lagea <lagea@student.s19.be>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/03 14:59:09 by lagea             #+#    #+#             */
/*   Updated: 2025/05/06 18:23:13 by lagea            ###   ########.fr       */
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
    _env.clear();
}

ConfigFile Webserv::getConfigFileObject() const
{
    return _config;
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
    catch (const std::exception &e)
    {
        throw std::runtime_error(e.what());
    }
}

void Webserv::initialiseEnv(char **env)
{
    int i = -1;

    while (env[++i])
    {
        std::stringstream ss(env[i]);
        std::string key;
        std::string val;
        getline(ss, key, '=');
        getline(ss, val);
        _env[key] = val;
    }
}