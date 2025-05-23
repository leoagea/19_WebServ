/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lagea <lagea@student.s19.be>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 13:16:56 by lagea             #+#    #+#             */
/*   Updated: 2025/04/29 19:26:14 by lagea            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include "config/configFileParser.hpp"
#include "server/TcpServer.hpp"
#include "server/ErrorPageGenerator.hpp"
#include "Webserv.hpp"
#include "server/DirectoryListing.hpp"
#include "../inc/Enum.h"
#include "../inc/Struct.h"

int main(int ac, char **av, char **env)
{
    std::string configPath;
    std::vector<int> ports;

    if (ac == 1)
        configPath = DEFAULT_PATH;
    else if (ac == 2)
        configPath = av[1];
    else
    {
        std::cerr << "Error: too many arguments" << std::endl;
        return 1;
    }

    Webserv data(configPath, env);
    try
    {
        data.initialiseConfig();
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
        return 1;
    }

    if (data.getConfigFileObject().isParsingFailed())
    {
        data.getConfigFileObject().printErrors();
        return 1;
    }

    std::vector<ServerBlock> serverBlocks = data.getConfigFileObject().getServerBlockVector();
    for (size_t i = 0; i < serverBlocks.size(); ++i)
        for (size_t j = 0; j < serverBlocks[i].getListeningPorts().size(); ++j)
            ports.push_back(serverBlocks[i].getListeningPortByIndex(j));

    TcpServer server(ports, data.getConfigFileObject(), data.getEnvMap());
    server.startServer();

    return 0;
}
