/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PathChecking.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lagea <lagea@student.s19.be>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 22:18:36 by lagea             #+#    #+#             */
/*   Updated: 2025/04/29 13:42:53 by lagea            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __PATHCHECKING_CPP__
#define __PATHCHECKING_CPP__

#include <string>
#include <sys/stat.h>
#include <unistd.h>

namespace PathChecking
{
    bool exist(std::string &path);
    bool isAbsolutePath(std::string &path);
    bool isFile(std::string &path);
    bool isDirectory(std::string &path);
    bool getReadPermission(std::string &path);
    bool getWritePermission(std::string &path);
    bool getExecPermission(std::string &path);
}

#endif