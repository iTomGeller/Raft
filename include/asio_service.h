
#ifndef _ASIO_SERVICE_H_
#define _ASIO_SERVICE_H_

#include <asio.hpp>
#include "delayed_task.h"
#include "delayed_task_scheduler.h"
#include "logger.h"
#include "pp_util.h"
#include "rpc_cli_factory.h"
#include "rpc_listener.h"

namespace raft
{

/**
  Declaring this to hide the dependency of asio.hpp from root header file, which can boost the compilation time
*/
class asio_service_impl;
class asio_rpc_client;
class asio_service : public delayed_task_scheduler, public rpc_client_factory
{
public:
    enum log_level
    {
        debug = 0x0,
        info,
        warnning,
        error
    };

public:
    asio_service();
    ~asio_service();

    __nocopy__(asio_service);

public:
    //纯虚函数存在的一个重要意义：避免对象切片 这里传的是ptr而不是值就可以避免对象切片
    virtual void schedule(ptr<delayed_task>& task, int32 milliseconds) __override__;
    virtual ptr<rpc_client> create_client(const std::string& endpoint) __override__;

    ptr<logger> create_logger(log_level level, const std::string& log_file);

    ptr<rpc_listener> create_rpc_listener(ushort listening_port, ptr<logger>& l);

    void stop();

private:
    virtual void cancel_impl(ptr<delayed_task>& task) __override__;

private:
    ptr<asio_service_impl> impl_;//这是pimpl idiom
};
} // namespace raft

#endif //_ASIO_SERVICE_H_