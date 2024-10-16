#include <asio.hpp>
#include "asio_service.h"
#include "iostream"
asio::io_service io_svc;
std::string host = "baidu.com", port = "80";
std::shared_ptr<asio_rpc_client> cli(std::make_shared<asio_rpc_client>(host, port, io_svc));
//声明一个instance必须要用shared_ptr 否则会 terminate called after throwing an instance of 'std::bad_weak_ptr'
int main() {
        //直接用字符串是不行的
        io_svc.run();
        bufptr buf = buffer::alloc(1024);
        buf->put("hello baidu!how are yout?");
        //不能传入unique_ptr作为参数
        cli->send(buf);
        //不能直接用private的变量
        //cli.connect();
}