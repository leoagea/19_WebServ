/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lagea <lagea@student.s19.be>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 13:16:56 by lagea             #+#    #+#             */
/*   Updated: 2024/12/16 13:16:57 by lagea            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server/TcpServer.hpp"

/* MAIN TEST VICENTE */

int main() {
    std::vector<int> ports = {8080, 9090, 10101, 2341, 5643, 2135, 9999, 9001, 65000};
    TcpServer server(ports);
    server.startServer();
    
    return 0;
}