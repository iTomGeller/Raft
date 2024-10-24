#ifndef _PP_UTIL_H_
#define _PP_UTIL_H_

#define __override__ override
#define __nocopy__(clazz)         \
private:                          \
    clazz(const clazz&) = delete; \
    clazz& operator=(const clazz&) = delete;

#define nilptr nullptr
#define __interface_body__(clazz) \
public:                           \
    clazz()                       \
    {                             \
    }                             \
    virtual ~clazz()              \
    {                             \
    }                             \
    __nocopy__(clazz)

#define auto_lock(lock) std::lock_guard<std::mutex> guard(lock)
#define recur_lock(lock) std::lock_guard<std::recursive_mutex> guard(lock)
#define read_lock(lock) std::shared_lock<std::shared_timed_mutex> rlock(lock)
#define write_lock(lock) std::lock_guard<std::shared_timed_mutex> wlock(lock)

#define sz_int sizeof(int32)
#define sz_ulong sizeof(ulong)
#define sz_byte sizeof(byte)
#endif //_PP_UTIL_H_