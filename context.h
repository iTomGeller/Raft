#ifndef _CONTEXT_H_
#define _CONTEXT_H_

#include "delayed_task_scheduler.h"
#include "events.h"
#include "logger.h"
#include "raft_params.h"
#include "rpc_cli_factory.h"
#include "rpc_listener.h"
#include "state_machine.h"
#include "state_mgr.h"

namespace raft
{
struct context
{
public:
    context(
        const ptr<state_mgr>& mgr,
        const ptr<state_machine>& m,
        const ptr<rpc_listener>& listener,
        const ptr<logger>& l,
        const ptr<rpc_client_factory>& cli_factory,
        const ptr<delayed_task_scheduler>& scheduler,
        const ptr<raft_event_listener>& event_listener,
        raft_params* params = nilptr)
        : state_mgr_(mgr),
          state_machine_(m),
          rpc_listener_(listener),
          logger_(l),
          rpc_cli_factory_(cli_factory),
          scheduler_(scheduler),
          event_listener_(event_listener),
          params_(params == nilptr ? new raft_params : params)
    {
    }

    __nocopy__(context);

public:
    ptr<state_mgr> state_mgr_;
    ptr<state_machine> state_machine_;
    ptr<rpc_listener> rpc_listener_;
    ptr<logger> logger_;
    ptr<rpc_client_factory> rpc_cli_factory_;
    ptr<delayed_task_scheduler> scheduler_;
    ptr<raft_event_listener> event_listener_;
    uptr<raft_params> params_;
};
} // namespace raft

#endif //_CONTEXT_H_
