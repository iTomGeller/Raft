#ifndef _MSG_BASE_H_
#define _MSG_BASE_H_

#include<iostream>
#include "basic_types.h"
#include "msg_type.h"
#include "pp_util.h"

namespace raft
{

class msg_base
{
public:
    msg_base(ulong term, msg_type type, int src, int dst) : term_(term), type_(type), src_(src), dst_(dst){}

    virtual ~msg_base(){}

    ulong get_term() const {
      return this->term_;
    }

    msg_type get_type() const {
      return this->type_;
    }

    int get_src() const {
      return this->src_;
    }

    int get_dst() const {
      return this->dst_;
    }


private:
    ulong term_;
    msg_type type_;
    int32 src_;
    int32 dst_;
};
} // namespace raft
#endif //_MSG_BASE_H_