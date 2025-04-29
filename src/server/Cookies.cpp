/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cookies.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lagea <lagea@student.s19.be>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 16:00:05 by lagea             #+#    #+#             */
/*   Updated: 2025/04/29 13:45:44 by lagea            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Cookies.hpp"

Cookies::Cookies()
{
}

Cookies::~Cookies()
{
}

std::string Cookies::generateSessionID()
{
    std::srand(time(NULL) + std::rand());
    std::stringstream ss;
    for (int i = 0; i < 32; i++)
    {
        int j = rand() % 127;
        while (j < 32)
            j = rand() % 127;
        if (j == ';' || j == '|')
            j = '/';
        ss << char(j);
    }
    return ss.str();
}

void Cookies::writeCookiesInDB(t_mapDB &data)
{
    std::fstream dbStream(COOKIE_DB_PATH, std::ios::out | std::ios::trunc);

    if (!dbStream.is_open())
    {
        TcpServer::generateLog(RED, "Failed to reached cookie DB", "ERROR");
        return;
    }

    t_mapDB::iterator it;
    for (it = data.begin(); it != data.end(); it++)
    {
        dbStream << it->second.login << "|" << it->second.sessionID << "|" << it->second.counter << std::endl;
    }

    dbStream.close();
}

t_mapDB Cookies::readDB()
{
    std::ifstream file(COOKIE_DB_PATH);
    std::string line;
    std::string id;
    std::string value;
    std::string login;
    t_mapDB map;

    if (!file.is_open())
    {
        TcpServer::generateLog(RED, "Failed to open cookies DB", "ERROR");
        return map;
    }

    while (getline(file, line))
    {
        size_t first = line.find('|');
        size_t second = line.find('|', first + 1);
        t_user user;
        if (first != std::string::npos && second != std::string::npos)
        {
            user.login = line.substr(0, first);
            user.sessionID = line.substr(first + 1, second - first - 1);
            user.counter = line.substr(second + 1, line.size());
            map[login] = user;
        }
    }

    return map;
}
