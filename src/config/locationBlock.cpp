/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   locationBlock.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmailleu <kmailleu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 17:05:04 by lagea             #+#    #+#             */
/*   Updated: 2025/05/06 19:10:47 by kmailleu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "locationBlock.hpp"

locationBlock::locationBlock(ServerBlock &server, std::vector<t_token> &vec, const ErrorReporter &reporter) : _server(server), _tokenVec(vec), _uri(""), _root(""),
                                                                                                              _index(""), _autoindex(-1), _isredirect(false), _redirect(-1, ""), _iscgi(false), _cgi(-1, ""), _cgipath(""),
                                                                                                              _allowedget(-1), _allowedpost(-1), _alloweddelete(-1), _allowedupload(-1), _reportError(reporter)
{
    parseAllLocationVariables();
}

locationBlock &locationBlock::operator=(const locationBlock &other)
{
    if (this != &other)
    {
        _server = other._server;
        _tokenVec = other._tokenVec;
        _uri = other._uri;
        _root = other._root;
        _index = other._index;
        _rootIndex = other._rootIndex;
        _autoindex = other._autoindex;
        _isredirect = other._isredirect;
        _redirect = other._redirect;
        _iscgi = other._iscgi;
        _cgi = other._cgi;
        _cgipath = other._cgipath;
        _allowedget = other._allowedget;
        _allowedpost = other._allowedpost;
        _alloweddelete = other._alloweddelete;
        _allowedupload = other._allowedupload;
        _reportError = other._reportError;
    }
    return *this;
}

locationBlock::~locationBlock()
{
}

// True if location block is about cgi
bool locationBlock::getCgi() const
{
    return _iscgi;
}

// return empty string if cgi, otherwise return uri like localhost/get
std::string locationBlock::getUri() const
{
    return _uri;
}

std::string locationBlock::getRootDirLoc() const
{
    return _root;
}

// return index name
std::string locationBlock::getIndexLoc() const
{
    return _index;
}

// return index concatenate with the root dir
std::string locationBlock::getRootIndexConcatenate() const
{
    return _rootIndex;
}

bool locationBlock::getAutoIndexLoc() const
{
    return _autoindex == 1;
}

bool locationBlock::getAllowedMethodGET() const
{
    return _allowedget == 1;
}

bool locationBlock::getAllowedMethodPOST() const
{
    return _allowedpost == 1;
}

bool locationBlock::getAllowedMethodDELETE() const
{
    return _alloweddelete == 1;
}

bool locationBlock::getAllowedMethodUPLOAD() const
{
    return _allowedupload == 1;
}

std::string locationBlock::getCgiScriptName() const
{
    return _cgi.second;
}

std::string locationBlock::getCgiPath() const
{
    return _cgipath;
}

bool locationBlock::getIsredirect() const
{
    return _isredirect;
}

int locationBlock::getRedirectReturnStatus() const
{
    return _redirect.first;
}

std::string locationBlock::getRedirectName() const
{
    return _redirect.second;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void locationBlock::parseAllLocationVariables()
{
    if (_tokenVec[0].type == keyword && _tokenVec[0].value == "location")
    {
        if (_tokenVec[2].type != openbracket)
        {
            _reportError(_tokenVec[2].index, "expected location string {}");
        }
        else
        {

            if (isCGI(_tokenVec[1].value, _tokenVec[1].index))
                _iscgi = true;
            else
            {
                if (_tokenVec[1].value[_tokenVec[1].value.size() - 1] != '/' && _tokenVec[3].type == keyword && _tokenVec[3].value == "return")
                    _isredirect = true;
            }
        }
    }
    _uri = _tokenVec[1].value;

    int i = 3;
    for (; i < (int)_tokenVec.size(); i++)
    {
        t_token current = _tokenVec[i];
        if (current.type == keyword && current.value == "root" && _tokenVec[i + 2].type == semicolon)
        {
            parseRootDir(_tokenVec[++i]);
            i++;
        }
        else if (current.type == keyword && current.value == "index" && _tokenVec[i + 2].type == semicolon)
        {
            parseIndex(_tokenVec[++i]);
            i++;
        }
        else if (current.type == keyword && current.value == "autoindex" && _tokenVec[i + 2].type == semicolon)
        {
            parseAutoIndex(_tokenVec[++i]);
            i++;
        }
        else if (current.type == keyword && current.value == "allowed_methods")
        {
            i++;
            while (_tokenVec[i].type != semicolon)
            {
                if (_tokenVec[i].type != string)
                    _reportError(_tokenVec[i].index, "expected only string");
                else
                    parseAllowedMethod(_tokenVec[i]);
                i++;
            }
        }
        else if (current.type == keyword && current.value == "include" && _tokenVec[i + 2].type == semicolon)
        {
            parseInclude(_tokenVec[++i]);
            i++;
        }
        else if (current.type == keyword && current.value == "cgi_param" && _tokenVec[i + 2].type == semicolon)
        {
            parseCgiScriptName(_tokenVec[++i]);
            i++;
        }
        else if (current.type == keyword && current.value == "return" && _tokenVec[i + 3].type == semicolon)
        {
            parseRedirect(_tokenVec[i + 1], _tokenVec[i + 2]);
            i += 3;
        }
        else
        {
            if (current.type == closebracket && i == (int)_tokenVec.size() - 1)
                break;
            _reportError(current.index, "unknown token");
            char stop = 0;
            while (_tokenVec[i].type != semicolon || (stop == 0 || stop == 1))
            {
                if (_tokenVec[i].type == openbracket)
                    stop = 1;
                else if (stop && _tokenVec[i].type == closebracket)
                    break;
                i++;
            }
        }
    }

    if (_root == "")
        _root = _server.getRootDir();
}

void locationBlock::parseRootDir(t_token &token)
{
    std::string path = token.value;

    if (_root == "")
    {
        if (PathChecking::exist(path))
            if (PathChecking::isDirectory(path))
                if (path[path.size() - 1] == '/')
                    _root = path;
                else
                    _root = path + "/";
            else
                _reportError(token.index, "is not directory");
        else
            _reportError(token.index, "path does not exist");
    }
    else
        _reportError(token.index, "root already defined");
}

void locationBlock::parseIndex(t_token &token)
{
    std::string path;
    if (_root == "")
        path = _server.getRootDir() + token.value;
    else
        path = _root + token.value;

    if (_index == "")
    {
        if (PathChecking::exist(path))
            if (PathChecking::isFile(path))
                if (PathChecking::getReadPermission(path))
                    _index = token.value;
                else
                    _reportError(token.index, "file has no read permission");
            else
                _reportError(token.index, "is not a file");
        else
            _reportError(token.index, "file does not exist");
    }
    else
        _reportError(token.index, "index already defined");
}

void locationBlock::parseAutoIndex(t_token &token)
{
    if (_autoindex == -1)
    {
        if (token.value == "on")
            _autoindex = true;
        else if (token.value == "off")
            _autoindex = false;
        else
            _reportError(token.index, "expected only on or off");
    }
    else
        _reportError(token.index, "autoindex already defined");
}

void locationBlock::parseAllowedMethod(t_token &token)
{
    if (token.value == "GET" && _allowedget == -1)
        _allowedget = 1;
    else if (token.value == "POST" && _allowedpost == -1)
        _allowedpost = 1;
    else if (token.value == "DELETE" && _alloweddelete == -1)
        _alloweddelete = 1;
    else if (token.value == "UPLOAD" && _allowedupload == -1)
        _allowedupload = 1;
    else if (_allowedget == 1 || _allowedpost == 1 || _alloweddelete == 1 || _allowedupload == 1)
        _reportError(token.index, "method already allowed");
    else
        _reportError(token.index, "expected only get post delete or upload");
}

void locationBlock::parseInclude(t_token &token)
{
    if (_cgi.first == -1)
    {
        if (token.type == keyword)
            if (token.value == "cgi_param")
            {
                _iscgi = true;
                if (_iscgi)
                    _cgi.first = true;
                else
                    _reportError(token.index, "cannot include param outside cgi location");
            }
            else
                _reportError(token.index, "expected cgi_param");
        else
            _reportError(token.index, "expected token cgi_param");
    }
    else
        _reportError(token.index, "include already defined");
}

void locationBlock::parseCgiScriptName(t_token &token)
{
    std::string path = _root + token.value;

    if (_cgi.second == "")
    {
        if (_cgi.first)
            if (PathChecking::exist(path))
                if (PathChecking::isFile(path))
                    if (PathChecking::getReadPermission(path))
                    {
                        _cgipath = path;
                        _cgi.second = token.value;
                    }
                    else
                        _reportError(token.index, "cgi script has no exec permission");
                else
                    _reportError(token.index, "cgi script is not a file");
            else
                _reportError(token.index, "cgi script does not exist");
        else
            _reportError(token.index, "not in cgi location");
    }
    else
        _reportError(token.index, "cgi_param already defined");
}

void locationBlock::parseRedirect(t_token &status, t_token &redirect)
{
    if (_redirect.first == -1 && _redirect.second == "")
    {
        if (status.type == number && redirect.type == string)
        {
            if (_isredirect)
            {
                int statuscode = atoi(status.value.c_str());

                if (statuscode != 301)
                    _reportError(status.index, "expected only 301 return code (HTTP Redirect)");
                else
                    _redirect.first = statuscode;

                _redirect.second = redirect.value;
                std::string servername = _server.getServerName();
                if (servername[servername.size() - 1] != '/' && redirect.value[0] == '/')
                    _uri = redirect.value;
                else if (servername[servername.size() - 1] != '/' && redirect.value[0] != '/')
                    _uri = redirect.value;
                else if (servername[servername.size() - 1] == '/' && redirect.value[0] != '/')
                    _uri = redirect.value;
                else if (servername[servername.size() - 1] == '/' && redirect.value[0] == '/')
                {
                    servername.erase(servername.size() - 1);
                    _uri = redirect.value;
                }
                else
                    _reportError(redirect.index, "failed to retrieve redirect location name");
            }
            else
                _reportError(status.index, "expected return token only in redirect location");
        }
    }
    else
        _reportError(status.index, "redirect already defined");
}

bool locationBlock::isCGI(std::string &path, int index)
{
    if (!PathChecking::isAbsolutePath(path))
    {
        if (path != ".py$")
        {
            _reportError(index, "cgi not supported expected only py");
            return false;
        }
        return true;
    }
    else
        return false;
}

std::ostream &operator<<(std::ostream &out, const locationBlock &obj)
{
    out << BMAGENTA << "Location Block   " << obj.getUri() << RESET << std::endl;

    out << MAGENTA << "Is CGI" << RESET << std::endl;
    out << "bool:  " << obj.getCgi() << std::endl;

    out << MAGENTA << "Root dir" << RESET << std::endl;
    out << "Root dir:  " << obj.getRootDirLoc() << std::endl;

    out << MAGENTA << "Index" << RESET << std::endl;
    out << "Index name:  '" << obj.getIndexLoc() << "'" << std::endl;

    out << MAGENTA << "Index concatenate" << RESET << std::endl;
    out << "Index path:  '" << obj.getRootIndexConcatenate() << "'" << std::endl;

    out << MAGENTA << "Auto index" << RESET << std::endl;
    out << "bool:  " << obj.getAutoIndexLoc() << std::endl;

    out << MAGENTA << "Allowed method" << RESET << std::endl;
    out << "get:  " << obj.getAllowedMethodGET() << "  "
        << "post:  " << obj.getAllowedMethodPOST() << "  "
        << "delete:  " << obj.getAllowedMethodDELETE() << "  "
        << "upload:  " << obj.getAllowedMethodUPLOAD() << std::endl;

    out << MAGENTA << "Cgi Script" << RESET << std::endl;
    out << "script name:  '" << obj.getCgiScriptName() << "'" << std::endl;
    out << "script path:  '" << obj.getCgiPath() << "'" << std::endl;

    out << MAGENTA << "Redirect" << RESET << std::endl;
    out << "is redirect:  '" << obj.getIsredirect() << "'" << std::endl;
    out << "redirect status:  '" << obj.getRedirectReturnStatus() << "'" << std::endl;
    out << "redirect name:  '" << obj.getRedirectName() << "'" << std::endl;

    out << std::endl;
    return out;
}

