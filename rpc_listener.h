#ifndef _RPC_LISTENER_H_
#define _RPC_LISTENER_H_
#include <iostream>
namespace raft
{
    class rpc_listener
    {
        virtual void listen() = 0;
        virtual void stop() = 0;
    };
} // namespace raft
#endif