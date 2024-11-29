#ifndef _RPC_CLI_H_
#define _RPC_CLI_H_
#include <exception>
#include "async.h"
#include "req_msg.h"
#include "resp_msg.h"
#include "rpc_exception.h"

namespace raft
{
using rpc_result = async_result<ptr<resp_msg>, ptr<rpc_exception>>;
using rpc_handler = rpc_result::handler_type;

class rpc_client
{
    __interface_body__(rpc_client);

public:
    virtual void send(ptr<req_msg>& req, rpc_handler& when_done) = 0;
};
} // namespace raft

#endif //_RPC_CLI_H_