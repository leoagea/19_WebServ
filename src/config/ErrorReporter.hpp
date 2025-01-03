/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ErrorReporter.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lagea <lagea@student.s19.be>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/26 15:19:33 by lagea             #+#    #+#             */
/*   Updated: 2025/01/03 15:06:52 by lagea            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __ERRORREPORTER_HPP__
# define __ERRORREPORTER_HPP__

#include <iostream>

class ConfigFile;

class ErrorReporter
{
    public:
        ErrorReporter();
        ErrorReporter(ConfigFile *);
        ~ErrorReporter();
        
        void operator()(unsigned int, const std::string &);

    private:
        ConfigFile *_parser;
};

#endif
