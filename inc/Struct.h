/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Struct.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lagea <lagea@student.s19.be>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/02 17:22:52 by lagea             #+#    #+#             */
/*   Updated: 2025/04/29 13:43:57 by lagea            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __STRUCT_H__
#define __STRUCT_H__

#include <string>
#include <sys/types.h>
#include "Enum.h"

struct s_info
{
    std::string name;
    std::string fullpath;
    off_t raw_size; // raw size in bytes
    time_t mtime;
    std::string format_size; // formated size into a string
    std::string format_time; // formated time into a string
};

typedef struct s_redirect
{
    int htppcode;
    std::string newpath;
} t_redirect;

typedef struct s_token
{
    e_tokenType type;
    std::string value;
    unsigned int index;
    unsigned int line;
} t_token;

typedef struct s_user
{
    std::string login;
    std::string sessionID;
    std::string counter;
} t_user;

#endif