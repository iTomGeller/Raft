
#include <asio.hpp>
#include <iostream>
#include <memory>
#include "buffer.h"
//#include "buffer.hxx"
#include "rpc_cli.h"
#include "rpc_listener.h"
using namespace raft;
// request header, ulong term (8), msg_type type (1), int32 src (4), int32 dst (4), ulong last_log_term (8), ulong
// last_log_idx (8), ulong commit_idx (8) + one int32 (4) for log data size
#define RPC_REQ_HEADER_SIZE 3 * 4 + 8 * 4 + 1

// response header ulong term (8), msg_type type (1), int32 src (4), int32 dst (4), ulong next_idx (8), bool accepted
// (1)
#define RPC_RESP_HEADER_SIZE 4 * 2 + 8 * 2 + 2

class asio_rpc_client : public rpc_client, public std::enable_shared_from_this<asio_rpc_client>
{
private:
    asio::io_service io_svc;

public:
    asio_rpc_client(std::string &host, std::string &port, asio::io_service &io_svc)
        : host_(host), port_(port), socket_(io_svc), resolver_(io_svc)
    {
    }
    virtual ~asio_rpc_client()
    {
        if (socket_.is_open())
        {
            socket_.close();
        }
    }

private:
    void connect(bufptr &buf)
    {
        std::shared_ptr<asio_rpc_client> self(this->shared_from_this());
        asio::ip::tcp::resolver::query q(host_, port_, asio::ip::tcp::resolver::all_matching);
        // async_connect返回下一个可以连接的itor
        resolver_.async_resolve(
                                q,
                                [self, this, &buf](std::error_code err, asio::ip::tcp::resolver::iterator itor) mutable {
                                    asio::async_connect(
                                                        socket_,
                                                        itor,
                                                        [self, &buf](std::error_code err, asio::ip::tcp::resolver::iterator itor) mutable {
                                                            if (!err) {
                                                                std::cout << "Connected Successfully\n";
                                                                self->send(buf);
                                                            }
                                                            else {
                                                                std::cout << "Connected failed\n";
                                                            }
                                                        }
                                    );
                                }
        );
    }
    // override关键词强制重写
public:
    virtual void send(bufptr &buf) override
    {
        // std::cout << socket_.is_open() << std::endl;
        if (!socket_.is_open()) {
            this->connect(buf);
            // std::cout << "socket is closed!";
        }
        else
        {
            std::shared_ptr<asio_rpc_client> self(this->shared_from_this());
            //因为我们发送buffer没有关于size的任何信息，所以出错。
            asio::async_write(
                socket_, 
                asio::buffer(buf->data(), buf->size()),
                [this, self](std::error_code err, ssize_t bytes_transferred) {
                    std::cout << "send msg successfully!\n";
                }
            );
        }
    }

private:
    // 把走io_svc的声明成private，传参只需传io_svc
    std::string host_;
    std::string port_;
    asio::ip::tcp::socket socket_;
    asio::ip::tcp::resolver resolver_;
};

class asio_rpc_session : public std::enable_shared_from_this<asio_rpc_session>
{
public:
    asio_rpc_session(asio::io_service &io) : socket_(io) {}
    ~asio_rpc_session() {
        stop();
    }

public:
    asio::ip::tcp::socket& socket() {
        return socket_;
    }
    void start() {
        this->read();
    }
    void stop() {
        if (socket_.is_open()) {
            socket_.close();
        }
    }

private:
    void read() {
        //自动调用move constructor
        /*  msg_type t = (msg_type)header_->get_byte();
            int32 src = header_->get_int();
            int32 dst = header_->get_int();
            ulong term = header_->get_ulong();
            ulong last_term = header_->get_ulong();
            ulong last_idx = header_->get_ulong();
            ulong commit_idx = header_->get_ulong();*/
        //如果传来的数据没有RPC_REQ_HEADER_SIZE大就会出错
        header_ = buffer::alloc(RPC_REQ_HEADER_SIZE);
        std::shared_ptr<asio_rpc_session> self(this->shared_from_this());
        asio::async_read(
            socket_,
            asio::buffer(header_->data(), RPC_REQ_HEADER_SIZE),
            [this, self](std::error_code err, ssize_t bytes_transferred) {
                if (!err) {
                    header_->pos(RPC_REQ_HEADER_SIZE - 4);
                    int data_size = header_->get_int();
                    std::cout << "the header size is :" << header_->size() << std::endl;
                    std::cout << "the data size in header is :" << data_size << std::endl;
                    if (data_size < 0 || data_size > 0x1000000) {
                        this->stop();
                        return;
                    }
                    log_data = buffer::alloc(data_size);
                    if (data_size == 0) {
                        std::cout << "oops!\n";
                        this->read_complete();
                        return;
                    }
                    asio::async_read(
                        socket_,
                        asio::buffer(log_data->data(), data_size),
                        [self, this](std::error_code err, ssize_t bytes_transferred) {
                            self->read_log_data(err);
                        }
                    );
                }
                else {
                  //std::cout << "bytes trans is :" << bytes_transferred << std::endl;
                  std::cout << "failed to read rpc header from socket due to error" << std::endl;
                  this->stop();
                }
            }
        );

    }
private:
  void read_complete() {
    header_->pos(0);
    byte t = (byte)header_->get_byte();
    int src = header_->get_int();
    int dst = header_->get_int();
    ulong term = header_->get_ulong();
    ulong last_term = header_->get_ulong();
    ulong last_idx = header_->get_ulong();
    ulong commit_idx = header_->get_ulong();
    std::cout << src << " " << dst << std::endl;
    //header_最后存的data_size没用，因为不同data大小不一样。
    if (log_data && header_->get_int()) {
      std::vector<bufptr> entry;
      while (log_data->size() > log_data->pos()) {
        ulong term = log_data->get_ulong();
        byte val_type = (byte)log_data->get_byte();
        int val_size = log_data->get_int();
        bufptr buf(buffer::alloc((size_t)val_size));
        log_data->get_buf(buf);
        std::cout << "the term of buffer is :" << term << std::endl;
        std::cout << "the val_type of buffer is : " << val_type << std::endl;
        std::cout << "the val_size of buffer is : " << val_size << std::endl;
        std::cout << "the data of buffer is : " << buf->get_int() << std::endl;  
      }
    }
    else {

    }
    /*  msg_type t = (msg_type)header_->get_byte();
        int32 src = header_->get_int();
        int32 dst = header_->get_int();
        ulong term = header_->get_ulong();
        ulong last_term = header_->get_ulong();
        ulong last_idx = header_->get_ulong();
        ulong commit_idx = header_->get_ulong();*/
        
  }
    void read_log_data(std::error_code err) {
        if (!err) {
          this->read_complete();
          return;
        }
        std::cout << "the log_data size is :" << log_data->size() << std::endl;
        std::cout << "failed to read rpc log data from socket due to error" << std::endl;
        this->stop();
    }
private:
asio::ip::tcp::socket socket_;
bufptr log_data;
bufptr header_;
};

class asio_rpc_listener : public rpc_listener, public std::enable_shared_from_this<asio_rpc_listener>
{
//port 是 ushort类型
public:
    asio_rpc_listener(asio::io_service& io, ushort port) : 
     io_svc(io),
     acceptor_(io, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), port)) 
      {}
    ~asio_rpc_listener()
    {
        stop();
    }

public:
    virtual void listen()
    {
        if (!acceptor_.is_open())
            return;
        this->start();
    }
    virtual void stop()
    {
        if (acceptor_.is_open())
            acceptor_.close();
    }

private:
    void start()
    {
        std::shared_ptr<asio_rpc_listener> self(this->shared_from_this());
        std::shared_ptr<asio_rpc_session> session(std::make_shared<asio_rpc_session>(io_svc));
        acceptor_.async_accept(
            session->socket(),
            [self, session](std::error_code err)
            {
                if (!err)
                {
                    session->start();
                }
                else
                {
                    std::cout << "No clients yet.....\n";
                }
                self->listen();
            });
    }

private:
    //这里的io_svc得声明成引用
    asio::io_service &io_svc;
    asio::ip::tcp::acceptor acceptor_;
    std::mutex session_lock;
};