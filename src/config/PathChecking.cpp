/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PathChecking.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lagea <lagea@student.s19.be>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 22:17:56 by lagea             #+#    #+#             */
/*   Updated: 2025/04/29 13:42:50 by lagea            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "PathChecking.hpp"
#include <iostream>
bool PathChecking::exist(std::string &path)
{
    struct stat fileInfo;
    return stat(path.c_str(), &fileInfo) == 0;
}

bool PathChecking::isAbsolutePath(std::string &path)
{
    return !path.empty() && path[0] == '/';
}

bool PathChecking::isFile(std::string &path)
{
    struct stat fileInfo;
    stat(path.c_str(), &fileInfo);
    return S_ISREG(fileInfo.st_mode);
}

bool PathChecking::isDirectory(std::string &path)
{
    struct stat fileInfo;
    stat(path.c_str(), &fileInfo);
    return S_ISDIR(fileInfo.st_mode);
}

bool PathChecking::getReadPermission(std::string &path)
{
    struct stat fileInfo;
    stat(path.c_str(), &fileInfo);
    return S_IRUSR & fileInfo.st_mode && !access(path.c_str(), R_OK);
}

bool PathChecking::getWritePermission(std::string &path)
{
    struct stat fileInfo;
    stat(path.c_str(), &fileInfo);
    return S_IWUSR & fileInfo.st_mode && !access(path.c_str(), W_OK);
}

bool PathChecking::getExecPermission(std::string &path)
{
    struct stat fileInfo;
    stat(path.c_str(), &fileInfo);
    return S_IXUSR & fileInfo.st_mode && !access(path.c_str(), X_OK);
}
