/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LogReporter.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lagea <lagea@student.s19.be>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/30 16:31:18 by lagea             #+#    #+#             */
/*   Updated: 2024/12/30 17:00:55 by lagea            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __LOGREPORTER_HPP__
# define __LOGREPORTER_HPP__

#include <iostream>

#include "Log.hpp"
#include "../../inc/Enum.h"

class LogReporter
{
    public:
        LogReporter(Log *log);
        ~LogReporter();
        
        void operator()(e_log, e_error, std::string);
        
        private:
            Log *_log;
};

#endif