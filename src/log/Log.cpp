/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Log.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lagea <lagea@student.s19.be>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/30 16:55:38 by lagea             #+#    #+#             */
/*   Updated: 2025/01/03 15:05:44 by lagea            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Log.hpp"

Log::Log()
{
}

Log::Log(ConfigFile *parser, int server) : _server(parser->getServerBlockByIndex(server)) 
{
    _error = _server.getErrorsLogStream();
    _access = _server.getAccessLogStream();
}

Log::~Log()
{
}

void Log::reportErrorLog(e_error level, std::string &msg)
{
    if (_error->is_open()){
        if (level == INFO || level == WARNING || level == ERROR || level == FATAL){
            _error->seekp(std::ios::end);
            (*_error) << CINFO << getTimeStamp() << " ";
            
            if (level == INFO)
                (*_error) << "[INFO] " << msg << RESET << std::endl;
            else if (level == WARNING)
                (*_error) << CWARNING << "[WARNING] " << msg << RESET << std::endl;
            else if (level == ERROR)
                (*_error) << CERROR << "[ERROR] " << msg << RESET << std::endl;
            else if (level == FATAL)
                (*_error) << CFATAL << "[FATAL] " << msg << RESET << std::endl;
        }
    }
}

void Log::reportAccessLog(e_error level, std::string &msg)
{
    if (_access->is_open()){
        if (level == INFO || level == WARNING || level == ERROR || level == FATAL){
            _access->seekp(std::ios::end);
            (*_access) << CINFO << getTimeStamp() << " ";
            
            if (level == INFO)
                (*_access) << "[INFO] " << msg << RESET << std::endl;
            else if (level == WARNING)
                (*_access) << CWARNING << "[WARNING] " << msg << RESET << std::endl;
            else if (level == ERROR)
                (*_access) << CERROR << "[ERROR] " << msg << RESET << std::endl;
            else if (level == FATAL)
                (*_access) << CFATAL << "[FATAL] " << msg << RESET << std::endl;
        }
    }
}

std::string Log::getTimeStamp()
{
    time_t now = time(NULL);
    struct tm *ltime = localtime(&now);

    char buffer[32];  
    strftime(buffer, sizeof(buffer), "[%Y-%m-%d %H:%M:%S]", ltime);

    return std::string(buffer);
}