/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Struct.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lagea <lagea@student.s19.be>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/02 17:22:52 by lagea             #+#    #+#             */
/*   Updated: 2025/01/02 17:23:41 by lagea            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __STRUCT_H__
# define __STRUCT_H__

#include <string>

struct s_info {
    std::string name;  // nom de l'entrée
    off_t raw_size;        // taille en octets
    time_t mtime;      // date de dernière modification (format time_t)
	std::string format_size; //formated size into a string
	std::string format_time; //formated time into a string
};

#endif