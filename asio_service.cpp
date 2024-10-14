#include <asio.hpp>
#include "iostream"
int main() {
    int a;
    asio::io_service io_svc_;
    asio::ip::tcp::resolver resolver_(io_svc_);
    asio::ip::tcp::socket socket_(io_svc_);
    std::string host = "baidu.com", port = "80";
    asio::ip::tcp::resolver::query q(host, port, asio::ip::tcp::resolver::all_matching);
    //以iterator()作为结尾
    //socket必须是引用
    //baidu.com 80端口才开放
    resolver_.async_resolve(
                q,
                [&socket_](std::error_code err, asio::ip::tcp::resolver::iterator itor) mutable
                {
                if (!err) {
                asio::async_connect(socket_, itor, [](std::error_code err, asio::ip::tcp::resolver::iterator itor){
                     if (!err) {
                            std::cout << "Connected successfully!" << std::endl;
                        } else {
                            std::cerr << "Connect error: " << err.message() << std::endl;
                        }
                });
                for (auto t = itor; t != asio::ip::tcp::resolver::iterator(); t++)
                    std::cout << t->endpoint().address() << std::endl;
                }
                else {
                    std::cout << "resolve error\n";
                }
                });
    io_svc_.run();
    socket_.close();
}