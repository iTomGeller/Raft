#include "snapshot.h"

using namespace raft;

ptr<snapshot> snapshot::deserialize(buffer& buf)
{
    ulong last_log_idx = buf.get_ulong();
    ulong last_log_term = buf.get_ulong();
    ulong size = buf.get_ulong();
    ptr<cluster_config> conf(cluster_config::deserialize(buf));
    return cs_new<snapshot>(last_log_idx, last_log_term, conf, size);
}

bufptr snapshot::serialize()
{
    bufptr conf_buf = last_config_->serialize();
    bufptr buf = buffer::alloc(conf_buf->size() + sz_ulong * 3);
    buf->put(last_log_idx_);
    buf->put(last_log_term_);
    buf->put(size_);
    buf->put(*conf_buf);
    buf->pos(0);
    return buf;
}