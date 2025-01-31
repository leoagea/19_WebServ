/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cookies.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lagea <lagea@student.s19.be>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 16:00:15 by lagea             #+#    #+#             */
/*   Updated: 2025/01/31 19:42:24 by lagea            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __COOKIES_HPP__
# define __COOKIES_HPP__

#include <string>
#include <sstream>
#include <cstdlib>
#include <fstream>
#include <map>
#include "TcpServer.hpp"

#define COOKIE_DB_PATH "var/www/db/cookies.csv"

typedef std::pair<std::string, std::string> t_pair;
typedef std::map<std::string, std::string> t_mapDB;

class Cookies
{
    public:
        Cookies();
        ~Cookies();
    
        void setCookieValue(const std::string &);
        void writeCookiesInDB(t_pair);
        
        std::string getSessionId() const;
        std::string getCounterValue() const;
        
    private:
        std::string _sessionID;
        std::string _value;
        
        void    generateSessionID();
        t_mapDB readDB(std::fstream &);
        void    writeDB(t_mapDB &);
};

#endif