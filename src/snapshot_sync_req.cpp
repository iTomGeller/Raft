#include "snapshot_sync_req.h"
#include <cstring>

using namespace raft;

ptr<snapshot_sync_req> snapshot_sync_req::deserialize(buffer& buf)
{
    ptr<snapshot> snp(snapshot::deserialize(buf));
    ulong offset = buf.get_ulong();
    bool done = buf.get_byte() == 1;
    byte* src = buf.data();
    if (buf.pos() < buf.size())
    {
        size_t sz = buf.size() - buf.pos();
        bufptr b = buffer::alloc(sz);
        ::memcpy(b->data(), src, sz);
        return cs_new<snapshot_sync_req>(snp, offset, std::move(b), done);
    }
    else
    {
        return cs_new<snapshot_sync_req>(snp, offset, buffer::alloc(0), done);
    }
}

bufptr snapshot_sync_req::serialize()
{
    bufptr snp_buf = snapshot_->serialize();
    bufptr buf = buffer::alloc(snp_buf->size() + sz_ulong + sz_byte + (data_->size() - data_->pos()));
    buf->put(*snp_buf);
    buf->put(offset_);
    buf->put(done_ ? (byte)1 : (byte)0);
    buf->put(*data_);
    buf->pos(0);
    return buf;
}