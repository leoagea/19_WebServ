/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lagea <lagea@student.s19.be>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 13:16:56 by lagea             #+#    #+#             */
/*   Updated: 2025/01/02 17:41:35 by lagea            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include "config/configFileParser.hpp"
#include "server/TcpServer.hpp"
#include "log/Log.hpp"
#include "log/LogReporter.hpp"
#include "errors/ErrorPageGenerator.hpp"
#include "server/DirectoryListing.hpp"
#include "../inc/Enum.h"
#include "../inc/Struct.h"

int main(int ac, char **av)
{
    std::string configPath;
    
    if(ac == 1)
        configPath = DEFAULT_PATH;
    else if (ac == 2)
        configPath = av[1];
    else{
        std::cerr << "Error: too many arguments" << std::endl;
        return 1;
    }
    
    ConfigFile config(configPath);
    try
    {
        config.loadConfFile();
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        return 1;
    }

    if (config.isParsingFailed()){
        config.printErrors();
        return 1;
    }

    Log log(&config, 0);
    LogReporter reportLog(&log);

        // reportLog(ACCESS, INFO, "Hello INFO");
    // reportLog(ACCESS, WARNING, "Hello WARNING");
    // reportLog(ACCESS, ERROR, "Hello ERROR");
    // reportLog(ACCESS, FATAL, "Hello FATAL");
    // reportLog(ERRORS, INFO, "Hello INFO");
    
    ServerBlock server = config.getServerBlockByIndex(0);
    ErrorPageGnerator generator(server);
    
    // std::cout << generator.generateErrorPageCode(403) << std::endl;
    
    std::string dir = ".";
    std::vector<s_info> listing;
    try
    {
        listing = DirectoryListing::listDirectory(dir);
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }
    
    for (std::vector<s_info>::const_iterator it = listing.begin(); it != listing.end(); ++it) {
        std::cout << it->format_time << " | "
                << it->format_size << " | "
        		<< it->name
                << std::endl;
    }


    
    // int ports = config.getServerBlockByIndex(0).getListeningPort();
    // TcpServer server(ports);
    // server.startServer();

    return 0;
}
