#include "peer.h"

using namespace raft;

void peer::send_req(ptr<req_msg>& req, rpc_handler& handler)
{
    ptr<rpc_result> pending = cs_new<rpc_result>(handler);
    rpc_handler h = [this, req, pending](ptr<resp_msg>& resp, const ptr<rpc_exception>& ex) mutable
    { this->handle_rpc_result(req, pending, resp, ex); };
    rpc_->send(req, h);
}

void peer::handle_rpc_result(
    ptr<req_msg>& req,
    ptr<rpc_result>& pending_result,
    ptr<resp_msg>& resp,
    const ptr<rpc_exception>& err)
{
    if (req->get_type() == msg_type::append_entries_request || req->get_type() == msg_type::install_snapshot_request)
    {
        set_free();
    }

    if (err == nilptr)
    {
        resume_hb_speed();
        pending_result->set_result(resp, ptr<rpc_exception>());
    }
    else
    {
        slow_down_hb();
        pending_result->set_result(ptr<resp_msg>(), err);
    }
}