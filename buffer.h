#ifndef BUFFER_H
#define BUFFER_H
#include <iostream>
#include <memory>
#include <cstring>

#include "basic_types.h"
#include "pp_util.h"
namespace raft
{

    class buffer;
    using bufptr = std::unique_ptr<buffer>; // 定义成unique_ptr不好传递

    // pos拿来判断 d拿来取数据
    class buffer
    {
    public:
        // static说明是属于这个类而不是任何一个实例
        static bufptr alloc(ssize_t size);
        static bufptr copy(const buffer& buf);
        
        ssize_t pos() const;
        byte *data() const; // 返回data所在的起始位置
        // 因为是用new申请以字节为单位的内存 所以是byte
        ssize_t size() const;
        void pos(ssize_t p);
        
        void put(byte b);
        void put(int val);
        void put(ulong val);
        void put(std::string str);
        void put(const buffer &buf);

        int get_int();
        std::string get_str(ssize_t len);
        ulong get_ulong();
        byte get_byte();
        void get_buf(bufptr& dst);
    };
} // namespace raft
#endif