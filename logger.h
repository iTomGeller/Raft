#ifndef _LOGGER_H_
#define _LOGGER_H_

#include <string>
#include "pp_util.h"

namespace raft
{
class logger
{
    __interface_body__(logger);

public:
    virtual void debug(const std::string& log_line) = 0;
    virtual void info(const std::string& log_line) = 0;
    virtual void warn(const std::string& log_line) = 0;
    virtual void err(const std::string& log_line) = 0;
};
} // namespace raft

#endif //_LOGGER_H_