#ifndef _STATE_MGR_H_
#define _STATE_MGR_H_

#include "cluster_config.h"
#include "log_store.h"
#include "srv_state.h"

namespace raft
{
class state_mgr
{
    __interface_body__(state_mgr);

public:
    virtual ptr<cluster_config> load_config() = 0;
    virtual void save_config(const cluster_config& config) = 0;
    virtual void save_state(const srv_state& state) = 0;
    virtual ptr<srv_state> read_state() = 0;
    virtual ptr<log_store> load_log_store() = 0;
    virtual int32 server_id() = 0;
    virtual void system_exit(const int exit_code) = 0;
};
} // namespace raft

#endif //_STATE_MGR_H_