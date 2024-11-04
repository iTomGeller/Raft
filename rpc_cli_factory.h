#ifndef _RPC_CLIENT_FACTORY_H_
#define _RPC_CLIENT_FACTORY_H_

#include "pp_util.h"
#include "ptr.h"
#include "rpc_cli.h"
namespace raft
{
class rpc_client_factory
{
    __interface_body__(rpc_client_factory);

public:
    virtual ptr<rpc_client> create_client(const std::string& endpoint) = 0;
};
} // namespace raft

#endif