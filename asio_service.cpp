#include <asio.hpp>
#include "asio_service.h"
#include "iostream"
// 声明一个instance必须要用shared_ptr 否则会 terminate called after throwing an instance of 'std::bad_weak_ptr'
int main() {
         /*  msg_type t = (msg_type)header_->get_byte();
            int32 src = header_->get_int();
            int32 dst = header_->get_int();
            ulong term = header_->get_ulong();
            ulong last_term = header_->get_ulong();
            ulong last_idx = header_->get_ulong();
            ulong commit_idx = header_->get_ulong();*/
  asio::io_service io_svc;
  std::string host = "localhost", port = "8080";

  std::shared_ptr<asio_rpc_client> cli(
      std::make_shared<asio_rpc_client>(host, port, io_svc));

  bufptr buf = buffer::alloc(RPC_REQ_HEADER_SIZE + 8 + 8 + 1);
  buf->put(byte(1));
  buf->put(127);
  buf->put(666);
  buf->put(ulong(1));
  buf->put(ulong(0));
  buf->put(ulong(0));
  buf->put(ulong(4));
  buf->put(8 + 1 + 4 + 4);
  /* ulong term = log_data->get_ulong();
    byte val_type = (byte)log_data->get_byte();
    int val_size = log_data->get_int();*/
  buf->put(ulong(1));
  buf->put(byte(1));
  buf->put(4);
  buf->put(233);
  //buf->put(12345);
        //不加pos = 0 就会出错，因为发送的时候是从data()开始的 put会改变pos
  buf->pos(0);
  std::cout << "the buffer size is :" << buf->size() << std::endl;
  cli->send(buf);
  io_svc.run();
}