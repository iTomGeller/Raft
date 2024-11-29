#ifndef _STATE_MACHINE_H_
#define _STATE_MACHINE_H_

#include "async.h"
#include "buffer.h"
#include "log_entry.h"
#include "snapshot.h"

namespace raft
{
class state_machine
{
    __interface_body__(state_machine);

public:
    virtual void commit(const ulong log_idx, buffer& data, const uptr<log_entry_cookie>& cookie) = 0;
    virtual void pre_commit(const ulong log_idx, buffer& data, const uptr<log_entry_cookie>& cookie) = 0;
    virtual void rollback(const ulong log_idx, buffer& data, const uptr<log_entry_cookie>& cookie) = 0;
    virtual void save_snapshot_data(snapshot& s, const ulong offset, buffer& data) = 0;
    virtual bool apply_snapshot(snapshot& s) = 0;
    virtual int read_snapshot_data(snapshot& s, const ulong offset, buffer& data) = 0;
    virtual ptr<snapshot> last_snapshot() = 0;
    virtual ulong last_commit_index() = 0;
    virtual void create_snapshot(snapshot& s, async_result<bool>::handler_type& when_done) = 0;
};
} // namespace raft

#endif //_STATE_MACHINE_H_