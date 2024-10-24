#ifndef _LOG_ENTRY_H_
#define _LOG_ENTRY_H_

#include "basic_types.h"
#include "buffer.h"
#include "log_val_type.h"
#include "pp_util.h"
#include "ptr.h"

namespace raft
{
//
// log_entry_cookie, which is used to store an in memory object for log entry.
// which could be the command for state machine so that when state machine on
// leader node is trying to commit the log entry, it does not need to deserialize
// the command from the log entry again.
//
class log_entry_cookie
{
public:
    log_entry_cookie(uint tag, const ptr<void>& value) : tag_(tag), value_(value) {}
    const ptr<void>& value() const {}
    uint tag() const {
      return tag_;
    }

private:
    uint tag_;
    ptr<void> value_;
};
//实现bufptr 与 log_entry互相转换
class log_entry
{
public:
    log_entry(ulong term,
              bufptr&& buff,
              log_val_type value_type = log_val_type::app_log) 
              : term_(term),
                value_type_(value_type),
                buff_(std::move(buff)),
                cookie_() {
                }

    __nocopy__(log_entry);

public:
    inline ulong get_term() const {
      return term_;
    }

    inline void set_term(ulong term) {
      term_ = term;
    }

    inline log_val_type get_val_type() const {
      return value_type_;
    }

    inline buffer& get_buf() const {
      if (!buff_) {
        throw std::runtime_error("get_buf cannot be called for a log_entry with nil buffer");
      }

      return *buff_;
    }

    inline void set_cookie(uint tag, const ptr<void>& value) {
        cookie_ = std::make_unique<log_entry_cookie>(tag, value);
    }

    inline const uptr<log_entry_cookie>& get_cookie() const {
        return cookie_;
    }

    inline bufptr serialize() {
        buff_->pos(0);
        bufptr buf = buffer::alloc(sizeof(ulong) + sizeof(char) + buff_->size());
        buf->put(term_);
        buf->put((static_cast<byte>(value_type_)));
        buf->put(*buff_);
        buf->pos(0);
        return buf;
    }

    inline static ptr<log_entry> deserialize(buffer& buf) {
        ulong term = buf.get_ulong();
        log_val_type t = static_cast<log_val_type>(buf.get_byte());
        bufptr data = buffer::copy(buf);
        return cs_new<log_entry>(term, std::move(data), t);
    }

    inline static ulong term_in_buffer(buffer& buf) {
        ulong term = buf.get_ulong();
        buf.pos(0); // reset the position
        return term;
    }

private:
    ulong term_;
    log_val_type value_type_;
    bufptr buff_;
    uptr<log_entry_cookie> cookie_;
};
} // namespace raft
#endif //_LOG_ENTRY_H_