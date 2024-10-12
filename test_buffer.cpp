#include "buffer.h"
#include <iostream>
using namespace raft;
int main() {
    auto ptr = buffer::alloc(1024);
    std::cout << "the pos of ptr is:" <<  ptr->pos() << std::endl;
    std::cout << "the size of buffer is:" << ptr->size() << std::endl;
    ptr->put(32);
    ptr->pos(0);
    std::cout << "the first value of buffer is:" << ptr->get_int() << std::endl;
}