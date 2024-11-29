#ifndef _CORNERSTONE_H_
#define _CORNERSTONE_H_

#include <algorithm>
#include <atomic>
#include <chrono>
#include <cinttypes>
#include <condition_variable>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <exception>
#include <fstream>
#include <functional>
#include <list>
#include <memory>
#include <mutex>
#include <random>
#include <shared_mutex>
#include <string>
#include <thread>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#ifdef max
#undef max
#endif

#ifdef min
#undef min
#endif

#include "asio_service.h"
#include "async.h"
#include "basic_types.h"
#include "buffer.h"
#include "cluster_config.h"
#include "context.h"
#include "delayed_task.h"
#include "delayed_task_scheduler.h"
#include "events.h"
#include "fs_log_store.h"
#include "log_entry.h"
#include "log_store.h"
#include "log_val_type.h"
#include "logger.h"
#include "msg_base.h"
#include "msg_type.h"
#include "peer.h"
#include "pp_util.h"
#include "ptr.h"
#include "raft_params.h"
#include "raft_server.h"
#include "req_msg.h"
#include "resp_msg.h"
#include "rpc_cli.h"
#include "rpc_cli_factory.h"
#include "rpc_exception.h"
#include "rpc_listener.h"
#include "snapshot.h"
#include "snapshot_sync_ctx.h"
#include "snapshot_sync_req.h"
#include "srv_config.h"
#include "srv_role.h"
#include "srv_state.h"
#include "state_machine.h"
#include "state_mgr.h"
#include "strfmt.h"
#include "timer_task.h"
#endif // _CORNERSTONE_H_