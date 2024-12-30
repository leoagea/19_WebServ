/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Log.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lagea <lagea@student.s19.be>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/30 16:17:13 by lagea             #+#    #+#             */
/*   Updated: 2024/12/30 17:41:02 by lagea            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __LOG_HPP__
# define __LOG_HPP__

#include <iostream>
#include <ostream>
#include <ctime>

#include "../config/configFileParser.hpp"
#include "../../inc/Enum.h"

class Log
{
    public:
        Log(ConfigFile *, int);
        ~Log();
        
        void reportErrorLog(e_error ,std::string &);
        void reportAccessLog(e_error ,std::string &);
        
    private:
        ServerBlock _server;
        
        std::ofstream *_error;
        std::ofstream *_access;

        std::string getTimeStamp();
};

#endif