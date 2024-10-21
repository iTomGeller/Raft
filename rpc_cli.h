#ifndef RPC_CLI_H
#define RPC_CLI_H
#include "buffer.h"
namespace raft
{
    class rpc_client
    {
    public:
        virtual void send(bufptr &buf) = 0;
    };
} // namespace raft
#endif