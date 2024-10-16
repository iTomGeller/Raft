
#include <asio.hpp>
#include <iostream>
#include <memory>
#include "buffer.h"
#include "rpc_cli.h"

using namespace raft;
// request header, ulong term (8), msg_type type (1), int32 src (4), int32 dst (4), ulong last_log_term (8), ulong
// last_log_idx (8), ulong commit_idx (8) + one int32 (4) for log data size
#define RPC_REQ_HEADER_SIZE 3 * 4 + 8 * 4 + 1

// response header ulong term (8), msg_type type (1), int32 src (4), int32 dst (4), ulong next_idx (8), bool accepted
// (1)
#define RPC_RESP_HEADER_SIZE 4 * 2 + 8 * 2 + 2

class asio_rpc_client : public rpc_client, public std::enable_shared_from_this<asio_rpc_client>{
private:
asio::io_service io_svc;
public:
        asio_rpc_client(std::string &host, std::string &port, asio::io_service &io_svc) 
        : host_(host), port_(port), socket_(io_svc), resolver_(io_svc) {
        }
        virtual ~asio_rpc_client() {
            if (socket_.is_open()) {
                socket_.close();
            }
        }

private:
void connect(bufptr buf) {
        std::shared_ptr<asio_rpc_client> self(this->shared_from_this());
        asio::ip::tcp::resolver::query q(host_, port_, asio::ip::tcp::resolver::all_matching);
        //async_connect返回下一个可以连接的itor 
        resolver_.async_resolve(q,
                               [this, self, buf](std::error_code err, asio::ip::tcp::resolver::iterator itor) {
                                if (!err) {
                                     asio::async_connect(socket_,itor,
                                [this, self, buf](std::error_code err1, asio::ip::tcp::resolver::iterator itor) {
                                if (!err1) {
                                    std::cout << "Connected Successfully";
                                    send(buf);
                                }
                                else {
                                    std::cout << "Connected failed";
                                }
                                });
                                }
                                });
}
//override关键词强制重写
public:
virtual void send(bufptr buf) {
        if (!socket_.is_open()) {
            this->connect(buf);
           //std::cout << "socket is closed!";
        }
        else {
        std::shared_ptr<asio_rpc_client> self(this->shared_from_this());
        asio::async_write(socket_, asio::buffer(buf->data(), buf->size()),
                          [this, self](std::error_code err, ssize_t bytes_transferred) {
                            std::cout << "send msg successfully!\n";
                          });
        }
}

private:
//把走io_svc的声明成private，传参只需传io_svc
std::string host_;
std::string port_;
asio::ip::tcp::socket socket_;
asio::ip::tcp::resolver resolver_;
};
