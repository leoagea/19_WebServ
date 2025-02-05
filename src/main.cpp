/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lagea <lagea@student.s19.be>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 13:16:56 by lagea             #+#    #+#             */
/*   Updated: 2025/02/05 18:17:48 by lagea            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include "config/configFileParser.hpp"
#include "server/TcpServer.hpp"
#include "errors/ErrorPageGenerator.hpp"
#include "Webserv.hpp"
#include "server/DirectoryListing.hpp"
#include "../inc/Enum.h"
#include "../inc/Struct.h"

int main(int ac, char **av, char **env)
{
    std::string         configPath;
    std::vector<int>    ports;
    
    if(ac == 1)
        configPath = DEFAULT_PATH;
    else if (ac == 2)
        configPath = av[1];
    else{
        std::cerr << "Error: too many arguments" << std::endl;
        return 1;
    }
    
    Webserv data(configPath, env);
    try
    {
        data.initialiseConfig();
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        return 1;
    }

    if (data.getConfigFileObject().isParsingFailed()){
        data.getConfigFileObject().printErrors();
        return 1;
    }
    
    //Exemple pour monter comment generer une page d erreur
    // std::cout << data.getErrorPageGenObject().generateErrorPageCode(403) << std::endl;
    
    for(size_t i = 0; i < data.getConfigFileObject().getServerBlockVector().size(); ++i)
        ports.push_back(data.getConfigFileObject().getServerBlockVector()[i].getListeningPort());

    TcpServer   server(ports, data.getConfigFileObject(), data.getEnvMap());
    server.startServer();

    return 0;
}
