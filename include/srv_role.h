#ifndef _SRV_ROLE_H_
#define _SRV_ROLE_H_

namespace raft
{
enum class srv_role
{
    follower = 0x1,
    candidate,
    leader
};
}

#endif