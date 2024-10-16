#include <asio.hpp>
#include "asio_service.h"
#include "iostream"
//声明一个instance必须要用shared_ptr 否则会 terminate called after throwing an instance of 'std::bad_weak_ptr'
int main() {
        asio::io_service io_svc;
        std::string host = "baidu.com", port = "80";
        std::shared_ptr<asio_rpc_client> cli(std::make_shared<asio_rpc_client>(host, port, io_svc));
        bufptr buf = buffer::alloc(1024);
        buf->put("hello baidu!how are yout?");
        cli->send(buf);
        io_svc.run();
}