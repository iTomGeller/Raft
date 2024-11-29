#ifndef _CS_EVENTS_H_
#define _CS_EVENTS_H_

#include "pp_util.h"

namespace raft
{
enum class raft_event
{
    become_leader = 1,
    become_follower,
    logs_catch_up
};

class raft_event_listener
{
    __interface_body__(raft_event_listener);

public:
    virtual void on_event(raft_event event) = 0;
};
} // namespace raft

#endif //_CS_EVENTS_H_