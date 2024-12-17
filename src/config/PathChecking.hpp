/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PathChecking.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lagea <lagea@student.s19.be>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 22:18:36 by lagea             #+#    #+#             */
/*   Updated: 2024/12/17 22:18:37 by lagea            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __PATHCHECKING_CPP__
# define __PATHCHECKING_CPP__

#include <string>
#include <sys/stat.h>

namespace PathChecking
{
    bool exist(std::string &path);
    bool isFile(std::string &path);
    bool isDirectory(std::string &path);
    bool getReadPermission(std::string &path);
    bool getWritePermission(std::string &path);
    bool getExecPermission(std::string &path);
}

#endif