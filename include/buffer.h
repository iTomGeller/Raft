#ifndef BUFFER_H
#define BUFFER_H
#include <iostream>
#include <memory>
#include <cstring>

#include "basic_types.h"
#include "pp_util.h"
#include "ptr.h"

namespace raft
{

    class buffer;
    //using bufptr = std::unique_ptr<buffer>; // 定义成unique_ptr不好传递
    using bufptr = uptr<buffer, void (*)(buffer*)>;
    // pos拿来判断 d拿来取数据
    class buffer
    {
       buffer() = delete;
    __nocopy__(buffer);
    public:
        // static说明是属于这个类而不是任何一个实例
        static bufptr alloc(size_t size);
        static bufptr copy(const buffer& buf);
        
        size_t size() const;
        size_t pos() const;
        void pos(size_t p);
        
        
        int32 get_int();
        ulong get_ulong();
        byte get_byte();
        //void get_buf(bufptr& dst);
        void get(bufptr& dst);
        const char * get_str();
        byte *data() const; // 返回data所在的起始位置
        // 因为是用new申请以字节为单位的内存 所以是byte

        void put(byte b);
        void put(int32 val);
        void put(ulong val);
        //void put(std::string str);
        void put(const std::string& str);
        void put(const buffer &buf);
    };
    std::ostream& operator<<(std::ostream& out, buffer& buf);
    std::istream& operator>>(std::istream& in, buffer& buf);
} // namespace raft
#endif