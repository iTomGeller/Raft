#include "buffer.h"

#define __init_block(p, s, t) \
    ((t *)(p))[0] = (t)s;     \
    ((t *)(p))[1] = 0;

#define __init_small_block(p, s) __init_block(p, s, ushort);
#define __init_big_block(p, s)  \
    __init_block(p, s, uint); \
    ((uint *)(p))[0] |= 0x80000000;

#define __is_big_block(p) (((uint *)(p))[0] & 0x80000000)

#define __pos_of_big_block(p) ((uint *)(p))[1]
#define __pos_of_small_block(p) ((ushort *)(p))[1]

#define __pos_of_block(p) (__is_big_block(p)) ? __pos_of_big_block(p) : __pos_of_small_block(p)
#define __size_of_block(p) (__is_big_block(p)) ? ((uint *)(p))[0] ^ 0x80000000 : ((ushort *)(p))[0]

#define __mv_block_pos(p, len) \
    __is_big_block(p) ? ((uint *)(p))[1] += len : ((ushort *)(p))[1] += (ushort)len;

#define __set_block_pos(p, len) \
    __is_big_block(p) ? ((uint *)(p))[1] = len : ((ushort *)(p))[1] = (ushort)len;
// 这里直接用pos_of_big/small_block 不然会出现运算顺序问题
#define __data_of_block(p) __is_big_block(p) ? (byte *)(((byte *)((uint *)(p) + 2)) + __pos_of_big_block(p)) : (byte *)(((byte *)((ushort *)(p) + 2)) + __pos_of_small_block(p))

using namespace raft;

static void free_buffer(buffer* buf)
{
    if (buf != nullptr)
    {
        delete[] reinterpret_cast<char*>(buf);
    }
}

bufptr buffer::alloc(size_t size)
{
    if (size >= 0x80000000)
    {
        throw std::out_of_range("size exceed the max size that raft::buffer could support");
    }

    if (size >= 0x8000)
    {
        bufptr buf(reinterpret_cast<buffer *>(new char[size + sizeof(uint) * 2]), &free_buffer);
        void *ptr = reinterpret_cast<void *>(buf.get());
        __init_big_block(ptr, size);
        return buf;
    }

    bufptr buf(reinterpret_cast<buffer *>(new char[size + sizeof(ushort) * 2]), &free_buffer);
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

size_t buffer::pos() const
{
    return size_t(__pos_of_block(this));
}

byte *buffer::data() const
{
    return __data_of_block(this);
}

size_t buffer::size() const
{
    return (size_t)(__size_of_block(this));
}

void buffer::pos(size_t p)
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
void buffer::put(int32 val)
{
    size_t pos_ = pos();
    size_t sz = size();
    byte *d = data();
    if (pos_ + sz_int > sz)
    {
        throw std::overflow_error("insufficient buffer to store int");
    }

    for (size_t i = 0; i < sz_int; i++)
    {
        // 转成byte取最低8位
        *(d + i) = (byte)(val >> (i * 8));
    }
    __mv_block_pos(this, sz_int);
}

void buffer::put(ulong val)
{
    size_t pos_ = pos();
    size_t sz = size();
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

void buffer::put(const std::string& str)
{
    size_t pos_ = pos();
    size_t sz = size();
    byte *d = data();
    size_t len = str.size();

    if (pos_ + len + 1 > sz)
    {
        throw std::overflow_error("insufficient buffer to store str");
    }

    for (int i = 0; i < len; i++)
    {
        // 转成byte取最低8位
        *(d + i) = str[i];
    }
    *(d + len) = (byte)0;
    __mv_block_pos(this, len + 1);
}

void buffer::put(const buffer &buf)
{
  size_t sz = size();
  size_t p = pos();
  size_t src_sz = buf.size();
  size_t src_p = buf.pos();
  if ((sz - p) < (src_sz - src_p))
  {
      throw std::overflow_error("insufficient buffer to hold the other buffer");
  }

  byte* d = data();
  byte* src = buf.data();
  ::memcpy(d, src, src_sz - src_p);
  __mv_block_pos(this, src_sz - src_p);
}


int32 buffer::get_int()
{
    size_t pos_ = pos();

    if (pos_ + sz_int > size())
    {
        throw std::overflow_error("insufficient buffer available for a int");
    }

    byte *d = data();
    int32 val = 0;
    for (size_t i = 0; i < sz_int; i++)
    {
        int32 byteval = (*(d + i)) << (i * 8);
        val += byteval;
    }

    __mv_block_pos(this, sz_int);
    return val;
}

ulong buffer::get_ulong()
{
    size_t pos_ = pos();

    if (pos_ + sz_ulong > size())
    {
        throw std::overflow_error("insufficient buffer available for a int");
    }

    byte *d = data();
    ulong val = 0;
    for (size_t i = 0; i < sz_ulong; i++)
    {
        ulong byteval = ulong((*(d + i))) << (i * 8); // 需要加ull，否则默认按int来左移
        val += byteval;
    }

    __mv_block_pos(this, sz_ulong);
    return val;
}

const char* buffer::get_str()
{
    byte *d = data();
    size_t i = 0;
    size_t p = pos();
    size_t s = size();
    while (* (d + i) && i < s - p) {
      i++;
    }
    if (i == 0 || p + i >= s) {
      return nilptr;
    }
    __mv_block_pos(this, i + 1);
    return reinterpret_cast<const char *>(d);
}

byte buffer::get_byte()
{
    size_t pos_ = pos();
    if (pos_ > size())
    {
        throw std::overflow_error("insufficient buffer available for a byte");
    }
    byte val = *data();
    __mv_block_pos(this, 1);
    return val;
}

void buffer::get(bufptr& dst) {
  size_t dst_size = dst->size();
  size_t src_size = this->size();
  if (pos() + dst_size > src_size) {
    throw std::overflow_error("insufficient buffer to hold the other buffer");
  }
  byte* d = dst->data();
  byte* src = data();
  ::memcpy(d, src, dst_size);
  __mv_block_pos(this, src_size);
}

std::ostream& raft::operator<<(std::ostream& out, buffer& buf)
{
    if (!out)
    {
        throw std::ios::failure("bad output stream.");
    }

    out.write(reinterpret_cast<char*>(buf.data()), buf.size() - buf.pos());

    if (!out)
    {
        throw std::ios::failure("write failed");
    }

    return out;
}

std::istream& raft::operator>>(std::istream& in, buffer& buf)
{
    if (!in)
    {
        throw std::ios::failure("bad input stream");
    }

    char* data = reinterpret_cast<char*>(buf.data());
    int size = buf.size() - buf.pos();
    in.read(data, size);

    if (!in)
    {
        throw std::ios::failure("read failed");
    }

    return in;
}