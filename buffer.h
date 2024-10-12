#define BUFFER_H
//#ifndef BUFFER_H
#include <iostream>
#include <memory>
#include <cstring>

namespace raft{

class buffer;
using bufptr =  std::unique_ptr<buffer>;

typedef unsigned char byte;
#define sz_int 4
#define sz_ulong 8
//pos拿来判断 d拿来取数据
class buffer{
public:
    //static说明是属于这个类而不是任何一个实例
    static bufptr alloc(ssize_t size);
    ssize_t pos() const;
    byte* data() const;//返回data所在的起始位置 
    //因为是用new申请以字节为单位的内存 所以是byte
    ssize_t size() const;
    void pos(ssize_t p);

    void put(int val);
    void put(ulong  val);
    void put(std::string str);
    void put(const buffer& buf);

    int get_int();
    std::string get_str(ssize_t len);
    ulong get_ulong();
    byte get_byte();

};
}//end namespace raft
//#endif