/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Enum.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lagea <lagea@student.s19.be>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/30 16:37:59 by lagea             #+#    #+#             */
/*   Updated: 2025/04/29 13:43:53 by lagea            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __ENUM_HPP__
#define __ENUM_HPP__

enum e_log
{
    ERRORS,
    ACCESS
};

enum e_error
{
    INFO,
    WARNING,
    ERROR,
    FATAL
};

enum e_allowed_methods
{
    GET = 1,
    POST = 2,
    DELETE = 3,
    UPLOAD = 4
};

enum e_tokenType
{
    keyword,
    openbracket,
    closebracket,
    semicolon,
    number,
    string
};

#endif