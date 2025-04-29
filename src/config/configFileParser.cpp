/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   configFileParser.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lagea <lagea@student.s19.be>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 13:20:44 by lagea             #+#    #+#             */
/*   Updated: 2025/04/29 19:19:16 by lagea            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "configFileParser.hpp"

ConfigFile::ConfigFile() : _filepath(""), _configFileVector(), _tokenizerString(""), _tokensVec(), _serverlist(), _errors()
{
}

ConfigFile::ConfigFile(std::string path) : _filepath(path), _configFileVector(), _tokenizerString(""), _tokensVec(), _serverlist(), _errors()
{
}

ConfigFile::~ConfigFile()
{
}

void ConfigFile::loadConfFile()
{
    if (!isConfPathValid(_filepath))
        throw std::runtime_error("Error: config file: wrong extension, expected .conf");

    std::fstream file(_filepath.c_str());

    if (!file.is_open())
        throw std::runtime_error("Error: config file: failed to open");

    std::string line;
    while (getline(file, line))
    {
        if (!line.size())
            continue;

        std::string::iterator it = std::find(line.begin(), line.end(), '#');
        if (it != line.end())
            line.erase(it, line.end());

        _tokenizerString += line;
        _configFileVector.push_back(line);
    }

    if (_tokenizerString.empty())
        throw std::runtime_error("Error: config file: empty file");

    parseConfFile();
}

void ConfigFile::reportError(int tokenIndex, const std::string &msg)
{
    _errors.insert(std::make_pair(tokenIndex, msg));
}

bool ConfigFile::isParsingFailed() const
{
    if (_errors.empty())
        return false;
    return true;
}

void ConfigFile::printErrors() const
{
    if (!_errors.empty())
    {
        std::map<int, std::string>::const_iterator found;

        std::cout << std::endl;
        std::vector<t_token>::const_iterator it = _tokensVec.begin();
        std::vector<std::string>::const_iterator line;
        unsigned int j = 0;

        for (line = _configFileVector.begin(); line != _configFileVector.end(); line++)
        {
            std::string str = *line;
            int i = 0;
            while (str[i])
            {

                if (str[i] != ' ' && str[i] != '\n')
                {
                    std::cout << *line;
                    bool print = true;

                    while (it != _tokensVec.end() && it->line == j)
                    {
                        std::ptrdiff_t index = std::distance(_configFileVector.begin(), line) + 1;
                        found = _errors.find(it->index);
                        if (found != _errors.end())
                        {
                            if (print)
                            {
                                std::cout << RED << "line: " << index << RESET;
                                print = !print;
                            }
                            std::cout << " " << RED << found->second << RESET;
                        }
                        it++;
                    }

                    j++;
                    std::cout << std::endl;
                    break;
                }
                i++;
            }
        }
    }
}

std::vector<ServerBlock> ConfigFile::getServerBlockVector() const
{
    return _serverlist;
}

// Throw a out of range execption if index is out of bound of vector
// Otherwise return the ServerBlock object at index
ServerBlock ConfigFile::getServerBlockByIndex(int index) const
{
    return _serverlist.at(index);
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void ConfigFile::parseConfFile()
{
    Tokenizer token(_tokenizerString);
    _tokensVec = token.getTokensVector();

    // Print all tokens
    //  std::cout << token << std::endl;

    splitServerBlock();
    checkPortDuplicate();
}

bool ConfigFile::isConfPathValid(std::string &path)
{
    std::string extension = ".conf";
    std::string::iterator it = std::find_end(path.begin(), path.end(), extension.begin(), extension.end());

    if (it == path.end())
        return false;
    return true;
}

void ConfigFile::splitServerBlock()
{
    ErrorReporter reporter(this);

    for (int i = 0; i < (int)_tokensVec.size(); i++)
    {
        if (_tokensVec[i].type == keyword && _tokensVec[i].value == "server")
        {
            if (_tokensVec[i + 1].type == openbracket)
            {
                i += 2;
                ServerBlock server(i - 2, _tokensVec, &i, reporter);
                // Print everything in the server for debugging purpose
                // std::cout << server << std::endl;e
                _serverlist.push_back(server);
            }
        }
    }
}

void ConfigFile::checkPortDuplicate()
{
    bool check = false;
    std::vector<ServerBlock> serverlist = getServerBlockVector();
    
    std::string errorMsg = "ports: ";
    for (size_t i = 0; i < serverlist.size(); i++){
        for (size_t j = i + 1; j < serverlist.size(); j++){
            std::vector<int> ports1 = serverlist[i].getListeningPorts();
            std::vector<int> ports2 = serverlist[j].getListeningPorts();
            for (size_t k = 0; k < ports1.size(); k++){
                for (size_t l = 0; l < ports2.size(); l++){
                    if (ports1[k] == ports2[l]){
                        std::stringstream ss;
                        ss << ports1[k];
                        errorMsg = ss.str() + " ";
                        check = true;
                    }
                }
            }
        }
    }
    errorMsg += "defined in multiple server blocks";

    if (check)
        reportError(0, errorMsg);
}