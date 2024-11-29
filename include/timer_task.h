#ifndef _TIMER_TASK_H_
#define _TIMER_TASK_H_

#include "delayed_task.h"

namespace raft
{
template <typename T>
class timer_task : public delayed_task
{
public:
    using executor = std::function<void(T)>;

    timer_task(executor& e, T ctx) : exec_(e), ctx_(ctx)
    {
    }

protected:
    virtual void exec() __override__
    {
        if (exec_)
        {
            exec_(ctx_);
        }
    }

private:
    executor exec_;
    T ctx_;
};

template <>
class timer_task<void> : public delayed_task
{
public:
    typedef std::function<void()> executor;

    explicit timer_task(executor& e) : exec_(e)
    {
    }

protected:
    virtual void exec() __override__
    {
        if (exec_)
        {
            exec_();
        }
    }

private:
    executor exec_;
};
} // namespace raft

#endif //_TIMER_TASK_H_