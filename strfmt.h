#ifndef _STRING_FORMATTER_H_
#define _STRING_FORMATTER_H_

#include <cstdio>
#include "pp_util.hxx"

namespace raft
{
template <int N>
class strfmt
{
public:
    strfmt(const char* fmt) : fmt_(fmt)
    {
    }

    template <typename... TArgs>
    const char* fmt(TArgs... args)
    {
        ::snprintf(buf_, N, fmt_, args...);
        return buf_;
    }

    __nocopy__(strfmt) 
private :
  char buf_[N];
  const char* fmt_;
};

typedef strfmt<100> sstrfmt;
typedef strfmt<200> lstrfmt;
} // namespace raft

#endif //_STRING_FORMATTER_H_