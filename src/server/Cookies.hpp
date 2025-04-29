/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cookies.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lagea <lagea@student.s19.be>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 16:00:15 by lagea             #+#    #+#             */
/*   Updated: 2025/04/29 13:45:48 by lagea            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __COOKIES_HPP__
#define __COOKIES_HPP__

#include <string>
#include <sstream>
#include <cstdlib>
#include <fstream>
#include <map>
#include "TcpServer.hpp"
#include "../../inc/Struct.h"

#define COOKIE_DB_PATH "var/www/db/cookies.csv"

typedef std::map<std::string, t_user> t_mapDB;

class Cookies
{
public:
    static std::string generateSessionID();
    static void writeCookiesInDB(t_mapDB &);
    t_mapDB readDB();

private:
    Cookies();
    ~Cookies();
};

#endif