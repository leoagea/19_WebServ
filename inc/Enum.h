/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Enum.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lagea <lagea@student.s19.be>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/30 16:37:59 by lagea             #+#    #+#             */
/*   Updated: 2025/01/02 17:58:44 by lagea            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __ENUM_HPP__
# define __ENUM_HPP__

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

enum e_allowed_methods{
    GET = 1 << 0,
    POST = 1 << 1,
    DELETE = 1 << 2,
    UPLOAD = 1 << 3
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