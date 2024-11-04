#ifndef _CLUSTER_CONFIG_HXX_
#define _CLUSTER_CONFIG_HXX_

#include <list>
#include "srv_config.h"

namespace raft
{
class cluster_config
{
public:
    cluster_config(ulong log_idx = 0L, ulong prev_log_idx = 0L)
        : log_idx_(log_idx), prev_log_idx_(prev_log_idx), servers_()
    {
    }

    ~cluster_config()
    {
    }

    __nocopy__(cluster_config);//禁用copy constructor

public:
    typedef std::list<ptr<srv_config>>::iterator srv_itor;
    typedef std::list<ptr<srv_config>>::const_iterator const_srv_itor;

    static ptr<cluster_config> deserialize(buffer& buf);

    inline ulong get_log_idx() const
    {
        return log_idx_;
    }

    inline void set_log_idx(ulong log_idx)
    {
        prev_log_idx_ = log_idx_;
        log_idx_ = log_idx;
    }

    inline ulong get_prev_log_idx() const
    {
        return prev_log_idx_;
    }

    inline std::list<ptr<srv_config>>& get_servers()
    {
        return servers_;
    }

    ptr<srv_config> get_server(int id) const
    {
        for (const_srv_itor it = servers_.begin(); it != servers_.end(); ++it)
        {
            if ((*it)->get_id() == id)
            {
                return *it;
            }
        }

        return ptr<srv_config>();
    }

    bufptr serialize();

private:
    ulong log_idx_;
    ulong prev_log_idx_;
    std::list<ptr<srv_config>> servers_;
};
} // namespace raft

#endif //_CLUSTER_CONFIG_H_