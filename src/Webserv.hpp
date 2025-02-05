/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lagea <lagea@student.s19.be>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/03 14:53:34 by lagea             #+#    #+#             */
/*   Updated: 2025/02/05 19:01:35 by lagea            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __WEBSERV_HPP__
# define __WEBSERV_HPP__

#include "config/configFileParser.hpp"

class Webserv
{
    public:
        Webserv(const std::string &, char **env);
        ~Webserv();
        
        ConfigFile getConfigFileObject() const;
        std::map<std::string, std::string> getEnvMap() const;
        
        void initialiseConfig();
        void initialiseErrorPageGenerator();
        void initialiseEnv(char **);

    private:
        ConfigFile _config;
        std::map<std::string, std::string> _env;
};

#endif