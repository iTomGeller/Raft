#include "buffer.h"
#include <cassert>
#include <random>
#include <iostream>
using namespace raft;

static void do_test(bufptr& buf);

int main() {
    auto buf = buffer::alloc(1024);
    do_test(buf);

    buf = buffer::alloc(0x8000);
    do_test(buf);

    buf = buffer::alloc(0x10000);
    do_test(buf);
}
static void do_test(bufptr& buf)
{
    std::random_device rd;
    std::default_random_engine engine(rd());
    std::uniform_int_distribution<int>distribution(1, 10000);
    auto rand = [distribution, engine]() mutable -> int {return distribution(engine);};
    
    std::vector<int> vals;
    for (int i = 0; i < 100; i++) {
        int val  = rand();
        vals.push_back(val);
        buf->put(val);
    }
    assert(buf->pos() == sz_int * 100);
}