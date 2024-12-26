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

// #include <iostream>
// #include "config/configFileParser.hpp"
// 
// int main(int ac, char **av)
// {
    // std::string configPath;
    // 
    // if(ac == 1)
        // configPath = DEFAULT_PATH;
    // else if (ac == 2)
        // configPath = av[1];
    // else{
        // std::cerr << "Error: too many arguments" << std::endl;
        // return 1;
    // }
    // 
    // ConfigFile config(configPath);
// }

#include "server/TcpServer.hpp"

/* MAIN TEST VICENTE */

int main() {
    std::vector<int> ports = {8080, 9090, 10101, 2341, 5643, 2135, 9999, 9001, 65000};
    TcpServer server(ports);
    server.startServer();
    
    return 0;
}