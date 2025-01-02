/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lagea <lagea@student.s19.be>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 13:16:56 by lagea             #+#    #+#             */
/*   Updated: 2024/12/26 14:57:59 by lagea            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include "server/TcpServer.hpp"
#include "config/configFileParser.hpp"

int main(int ac, char **av)
{
    std::string configPath;
    std::vector<int> ports;
    
    if(ac == 1)
        configPath = DEFAULT_PATH;
    else if (ac == 2)
        configPath = av[1];
    else{
        std::cerr << "Error: too many arguments" << std::endl;
        return 1;
    }
    
    ConfigFile config(configPath);

    for(size_t i = 0; i < config.getServerBlockVector().size(); ++i)
        ports.push_back(config.getServerBlockVector()[i].getListeningPortByIndex(0));

    TcpServer server(ports);
    server.startServer();

    return 0;
}