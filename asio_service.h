#include <asio.hpp>
#include "iostream"
int main() {
    int a;
    asio::io_service io_svc_;
    io_svc_.run();
}