/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lagea <lagea@student.s19.be>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 13:16:56 by lagea             #+#    #+#             */
/*   Updated: 2024/12/17 16:39:09 by lagea            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include "config/configFileParser.hpp"

int main(int ac, char **av)
{
    std::string defaultConfigFilePath = "config/config.conf";
    
    if(ac == 1)
        ConfigFile config(defaultConfigFilePath);
    else if (ac == 2)
        ConfigFile config(av[1]);
    else{
        std::cerr << "Error: too many arguments" << std::endl;
    }
}