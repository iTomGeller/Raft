#ifndef _CS_PTR_H_
#define _CS_PTR_H_
#include <memory>
namespace raft
{
template <typename T>
using ptr = std::shared_ptr<T>;

template <typename T>
using wptr = std::weak_ptr<T>;
//----->为什么要有deleter?
template <typename T, typename Deleter = std::default_delete<T>>
using uptr = std::unique_ptr<T, Deleter>;

template <typename T, typename... TArgs>
inline ptr<T> cs_new(TArgs&&... args)
{
    return std::make_shared<T>(std::forward<TArgs>(args)...);
}
} // namespace raft
#endif //_CS_PTR_H_