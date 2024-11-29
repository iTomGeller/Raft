#ifndef _SNAPSHOT_SYNC_REQ_H_
#define _SNAPSHOT_SYNC_REQ_H_

#include "snapshot.h"

namespace raft
{
class snapshot_sync_req
{
public:
    snapshot_sync_req(const ptr<snapshot>& s, ulong offset, bufptr&& buf, bool done)
        : snapshot_(s), offset_(offset), data_(std::move(buf)), done_(done)
    {
    }

    __nocopy__(snapshot_sync_req);

public:
    static ptr<snapshot_sync_req> deserialize(buffer& buf);

    snapshot& get_snapshot() const
    {
        return *snapshot_;
    }

    ulong get_offset() const
    {
        return offset_;
    }

    buffer& get_data() const
    {
        return *data_;
    }

    bool is_done() const
    {
        return done_;
    }

    bufptr serialize();

private:
    ptr<snapshot> snapshot_;
    ulong offset_;
    bufptr data_;
    bool done_;
};
} // namespace raft

#endif //_SNAPSHOT_SYNC_REQ_H_