/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cookies.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lagea <lagea@student.s19.be>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 16:00:05 by lagea             #+#    #+#             */
/*   Updated: 2025/01/31 20:11:14 by lagea            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Cookies.hpp"

Cookies::Cookies() : _sessionID(""), _value("")
{
    generateSessionID();
}

Cookies::~Cookies()
{
}

void Cookies::setCookieValue(const std::string &val)
{
    _value = val;
}

void Cookies::writeCookiesInDB(t_pair pair)
{
    std::fstream dbStream(COOKIE_DB_PATH);
    
    if (!dbStream.is_open()){
        TcpServer::generateLog(RED, "Failed to reached cookie DB", "ERROR");
        return;
    }
    
    std::cout << "sessionID: "  << "'" << _sessionID << "'" << "  pair first: " << "'" << pair.first << "'" << std::endl;
    if (_sessionID != pair.first){
        dbStream.close();
        return;
    }
    
    if (_value == pair.second){
        dbStream.close();
        return;
    }

    // t_mapDB db = readDB(dbStream);
    
    // db[pair.first] = pair.second;
    _value = pair.second;
    
    // writeDB(db);
    
    dbStream.close();
}

std::string Cookies::getSessionId() const
{
    return _sessionID;
}

std::string Cookies::getCounterValue() const
{
    if (_value.empty())
        return "0";
    return _value;
}

/////////////////////// Private Functions ///////////////////////
void Cookies::generateSessionID()
{
    std::srand(time(NULL) + std::rand());
    std::stringstream ss;
    for(int i = 0;i < 32;i++)
    {
            int j = rand() % 127;
            while(j < 32)
                    j = rand() % 127;
            if (j == ';')
                j = '/';
            ss << char(j);
    }
    _sessionID = ss.str();
}

t_mapDB Cookies::readDB(std::fstream &file)
{
    std::string line;
    std::string id;
    std::string value;
    t_mapDB map;
    
    while(getline(file, line)){
        size_t pos = line.find('|');
        if (pos != std::string::npos){
            id = line.substr(0, pos);
            value = line.substr(pos + 1, line.size());
            map[id] = value;
        }
    }

    return map;
}

void Cookies::writeDB(t_mapDB &db)
{
    std::ofstream out(COOKIE_DB_PATH);
    t_mapDB::iterator it;
    
    
    for (it = db.begin(); it != db.end(); it++){
        out << it->first << "|" << it->second << std::endl;
    }
    out.close();
}
