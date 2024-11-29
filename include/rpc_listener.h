#ifndef _RPC_LISTENER_H_
#define _RPC_LISTENER_H_

#include "pp_util.h"
#include "ptr.h"

namespace raft
{
// for backward compatibility
class raft_server;
typedef raft_server msg_handler;

class rpc_listener
{
    __interface_body__(rpc_listener);

public:
    virtual void listen(ptr<msg_handler>& handler) = 0;
    virtual void stop() = 0;
};
} // namespace raft

#endif //_RPC_LISTENER_H_