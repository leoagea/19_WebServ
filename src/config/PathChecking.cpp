/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PathChecking.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lagea <lagea@student.s19.be>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 22:17:56 by lagea             #+#    #+#             */
/*   Updated: 2024/12/17 22:18:29 by lagea            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "PathChecking.hpp"

bool PathChecking::exist(std::string &path){
    struct stat fileInfo;
    return stat(path.c_str(), &fileInfo) == 0;
}

bool PathChecking::isFile(std::string &path){
    struct stat fileInfo;
    stat(path.c_str(), &fileInfo);
    return S_ISREG(fileInfo.st_mode);
}

bool PathChecking::isDirectory(std::string &path){
    struct stat fileInfo;
    stat(path.c_str(), &fileInfo);
    return S_ISDIR(fileInfo.st_mode);
}

bool PathChecking::getReadPermission(std::string &path){
    struct stat fileInfo;
    stat(path.c_str(), &fileInfo);
    return S_IRUSR & fileInfo.st_mode;
}

bool PathChecking::getWritePermission(std::string &path){
    struct stat fileInfo;
    stat(path.c_str(), &fileInfo);
    return S_IWUSR & fileInfo.st_mode;
}

bool PathChecking::getExecPermission(std::string &path){
    struct stat fileInfo;
    stat(path.c_str(), &fileInfo);
    return S_IXUSR & fileInfo.st_mode;
}
