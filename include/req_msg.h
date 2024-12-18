#ifndef _REG_MSG_H_
#define _REG_MSG_H_

#include <vector>

#include "msg_base.h"
#include "basic_types.h"
#include "pp_util.h"
#include "ptr.h"
#include "log_entry.h"

namespace raft
{
class req_msg : public msg_base
{
public:
    req_msg(ulong term,
            msg_type type,
            int32 src,
            int32 dst,
            ulong last_log_term,
            ulong last_log_idx,
            ulong commit_idx)
        : msg_base(term, type, src, dst),
          last_log_term_(last_log_term),
          last_log_idx_(last_log_idx),
          commit_idx_(commit_idx),
          log_entries_() {

          }

    virtual ~req_msg() __override__ {

    }

    __nocopy__(req_msg);

public:
    ulong get_last_log_idx() const {
        return last_log_idx_;
    }

    ulong get_last_log_term() const {
        return last_log_term_;
    }

    ulong get_commit_idx() const {
        return commit_idx_;
    }

    std::vector<ptr<log_entry>>& log_entries() {
        return log_entries_;
    }

private:
    ulong last_log_term_;
    ulong last_log_idx_;
    ulong commit_idx_;
    std::vector<ptr<log_entry>> log_entries_;
};
} // namespace raft

#endif //_REG_MSG_HXX_