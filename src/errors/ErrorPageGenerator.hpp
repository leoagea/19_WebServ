/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ErrorPageGenerator.hpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lagea <lagea@student.s19.be>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/02 14:34:04 by lagea             #+#    #+#             */
/*   Updated: 2025/01/03 15:07:48 by lagea            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __ERRORPAGEGENERATOR_HPP__
# define __ERRORPAGEGENERATOR_HPP__

#include <map>
#include <iostream>
#include <fstream>
#include <sstream>

#include "../config/serverBlock.hpp"

class ErrorPageGnerator
{
    public:
        ErrorPageGnerator();
        ErrorPageGnerator(ServerBlock &);
        ~ErrorPageGnerator();

        std::string generateErrorPageCode(int);
        
    private:
        std::string _htmlcode;        
        std::map<int, std::string> _errors;

        void generateHtmlCode(int, std::string);
        std::string readHtmlCode(std::string &);
};

#endif