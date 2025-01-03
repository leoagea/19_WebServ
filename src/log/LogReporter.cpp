/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LogReporter.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lagea <lagea@student.s19.be>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/30 16:56:21 by lagea             #+#    #+#             */
/*   Updated: 2025/01/03 15:07:37 by lagea            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "LogReporter.hpp"

LogReporter::LogReporter()
{
}

LogReporter::LogReporter(Log *log) : _log(log)
{
}

LogReporter::~LogReporter()
{
}

void LogReporter::operator()(e_log mod, e_error level, std::string msg)
{
    if (_log && mod == ACCESS)
        _log->reportAccessLog(level, msg);
    else if (_log && mod == ERRORS)
        _log->reportErrorLog(level, msg);
}
