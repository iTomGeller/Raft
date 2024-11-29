#ifndef _RESP_MSG_H_
#define _RESP_MSG_H_

#include "msg_base.h"

namespace raft
{
class resp_msg : public msg_base
{
public:
    resp_msg(ulong term,
             msg_type type,
             int32 src,
             int32 dst,
             ulong next_idx = 0L,
             bool accepted = false)
        : msg_base(term, type, src, dst),
          next_idx_(next_idx),
          accepted_(accepted) {
          }

    __nocopy__(resp_msg);

public:
    ulong get_next_idx() const {
        return next_idx_;
    }

    bool get_accepted() const {
        return accepted_;
    }

    void accept(ulong next_idx) {
        next_idx_ = next_idx; //使得leader 从 不同follower的next_idx开始发消息
        accepted_ = true;
    }

private:
    ulong next_idx_;
    bool accepted_;
};
} // namespace raft

#endif //_RESP_MSG_H_