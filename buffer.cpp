#include "buffer.h"

typedef unsigned int uint32;
#define __init_block(p, s, t) \
    ((t *)(p))[0] = (t)s;     \
    ((t *)(p))[1] = 0;

#define __init_small_block(p, s) __init_block(p, s, ushort);
#define __init_big_block(p, s)  \
    __init_block(p, s, uint32); \
    ((uint32 *)(p))[0] |= 0x80000000;

#define __is_big_block(p) (((uint32 *)(p))[0] & 0x80000000)

#define __pos_of_big_block(p) ((uint32 *)(p))[1]
#define __pos_of_small_block(p) ((ushort *)(p))[1]

#define __pos_of_block(p) (__is_big_block(p)) ? __pos_of_big_block(p) : __pos_of_small_block(p)
#define __size_of_block(p) (__is_big_block(p)) ? ((uint32 *)(p))[0] ^ 0x80000000 : ((ushort *)(p))[0]

#define __mv_block_pos(p, len) \
    __is_big_block(p) ? ((uint32 *)(p))[1] += len : ((ushort *)(p))[1] += (ushort)len;

#define __set_block_pos(p, len) \
    __is_big_block(p) ? ((uint32 *)(p))[1] = len : ((ushort *)(p))[1] = (ushort)len;
// 这里直接用pos_of_big/small_block 不然会出现运算顺序问题
#define __data_of_block(p) __is_big_block(p) ? (byte *)(((byte *)((uint32 *)(p) + 2)) + __pos_of_big_block(p)) : (byte *)(((byte *)((ushort *)(p) + 2)) + __pos_of_small_block(p))
using namespace raft;
bufptr buffer::alloc(ssize_t size)
{
    if (size >= 0x80000000)
    {
        throw std::out_of_range("size exceed the max size that raft::buffer could support");
    }

    if (size >= 0x8000)
    {
        bufptr buf(reinterpret_cast<buffer *>(new char[size + sizeof(uint32) * 2]));
        void *ptr = reinterpret_cast<void *>(buf.get());
        __init_big_block(ptr, size);
        return buf;
    }

    bufptr buf(reinterpret_cast<buffer *>(new char[size + sizeof(ushort) * 2]));
    void *ptr = reinterpret_cast<void *>(buf.get());
    __init_small_block(ptr, size);
    return buf;
}

bufptr buffer::copy(const buffer& buf)
//是deep copy
{
    //size包括header 而alloc里面的参数size是不包括header的
    bufptr other = alloc(buf.size() - buf.pos());
    other->put(buf);
    other->pos(0);
    return other;
}

ssize_t buffer::pos() const
{
    return ssize_t(__pos_of_block(this));
}

byte *buffer::data() const
{
    return __data_of_block(this);
}

ssize_t buffer::size() const
{
    return (ssize_t)(__size_of_block(this));
}

void buffer::pos(ssize_t p)
{
    __set_block_pos(this, p);
}

void buffer::put(byte b)
{
    if (size() - pos() < sz_byte)
    {
        throw std::overflow_error("insufficient buffer to store byte");
    }

    byte* d = data();
    *d = b;
    __mv_block_pos(this, sz_byte);
}
// 逆序存数据
void buffer::put(int val)
{
    ssize_t pos_ = pos();
    ssize_t sz = size();
    byte *d = data();
    if (pos_ + sz_int > sz)
    {
        throw std::overflow_error("insufficient buffer to store int");
    }

    for (ssize_t i = 0; i < sz_int; i++)
    {
        // 转成byte取最低8位
        *(d + i) = (byte)(val >> (i * 8));
    }
    __mv_block_pos(this, sz_int);
}

void buffer::put(ulong val)
{
    ssize_t pos_ = pos();
    ssize_t sz = size();
    byte *d = data();
    if (pos_ + sz_ulong > sz)
    {
        throw std::overflow_error("insufficient buffer to store ulong");
    }

    for (int i = 0; i < sz_ulong; i++)
    {
        // 转成byte取最低8位
        *(d + i) = (byte)(val >> (i * 8));
    }
    __mv_block_pos(this, sz_ulong);
}

void buffer::put(std::string str)
{
    ssize_t pos_ = pos();
    ssize_t sz = size();
    byte *d = data();
    ssize_t len = str.size();

    if (pos_ + len > sz)
    {
        throw std::overflow_error("insufficient buffer to store str");
    }

    for (int i = 0; i < len; i++)
    {
        // 转成byte取最低8位
        *(d + i) = str[i];
    }

    __mv_block_pos(this, len);
}

void buffer::put(const buffer &buf)
{
  ssize_t sz = size();
  ssize_t p = pos();
  ssize_t src_sz = buf.size();
  ssize_t src_p = buf.pos();
  if ((sz - p) < (src_sz - src_p))
  {
      throw std::overflow_error("insufficient buffer to hold the other buffer");
  }

  byte* d = data();
  byte* src = buf.data();
  ::memcpy(d, src, src_sz - src_p);
  __mv_block_pos(this, src_sz - src_p);
}

int buffer::get_int()
{
    ssize_t pos_ = pos();

    if (pos_ + sz_int > size())
    {
        throw std::overflow_error("insufficient buffer available for a int");
    }

    byte *d = data();
    int val = 0;
    for (ssize_t i = 0; i < sz_int; i++)
    {
        int byteval = (*(d + i)) << (i * 8);
        val += byteval;
    }

    __mv_block_pos(this, sz_int);
    return val;
}

ulong buffer::get_ulong()
{
    ssize_t pos_ = pos();

    if (pos_ + sz_ulong > size())
    {
        throw std::overflow_error("insufficient buffer available for a int");
    }

    byte *d = data();
    ulong val = 0;
    for (ssize_t i = 0; i < sz_ulong; i++)
    {
        ulong byteval = ulong((*(d + i))) << (i * 8); // 需要加ull，否则默认按int来左移
        val += byteval;
    }

    __mv_block_pos(this, sz_ulong);
    return val;
}

std::string buffer::get_str(ssize_t len)
{
    ssize_t pos_ = pos();

    if (pos_ + len > size())
    {
        throw std::overflow_error("insufficient buffer available for the string");
    }

    byte *d = data();
    std::string str;
    for (int i = 0; i < len; i++)
    {
        str += *(d + i);
    }

    __mv_block_pos(this, len);
    return str;
}

byte buffer::get_byte()
{
    ssize_t pos_ = pos();
    if (pos_ > size())
    {
        throw std::overflow_error("insufficient buffer available for a byte");
    }

    __mv_block_pos(this, 1);
    return *data();
}

void buffer::get_buf(bufptr& dst) {
  ssize_t dst_size = dst->size();
  ssize_t src_size = this->size();
  if (pos() + dst_size > src_size) {
    throw std::overflow_error("insufficient buffer to hold the other buffer");
  }
  byte* d = dst->data();
  byte* src = data();
  ::memcpy(d, src, dst_size);
  __mv_block_pos(this, src_size);
}