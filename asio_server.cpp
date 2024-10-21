#include <asio.hpp>
#include "asio_service.h"
#include "iostream"
// 声明一个instance必须要用shared_ptr 否则会 terminate called after throwing an instance of 'std::bad_weak_ptr'
int main()
{
        asio::io_service io_svc;
        std::shared_ptr<asio_rpc_listener> srv(std::make_shared<asio_rpc_listener>(io_svc, 8080));
        srv->listen();
        io_svc.run();
}