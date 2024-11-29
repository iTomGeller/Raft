#ifndef _DELAYED_TASK_SCHEDULER_H_
#define _DELAYED_TASK_SCHEDULER_H_

#include "basic_types.h"
#include "delayed_task.h"
#include "pp_util.h"
#include "ptr.h"

namespace raft
{
class delayed_task_scheduler
{
    __interface_body__(delayed_task_scheduler);

public:
    virtual void schedule(ptr<delayed_task>& task, int32 milliseconds) = 0;

    void cancel(ptr<delayed_task>& task)
    {
        cancel_impl(task);
        task->cancel();
    }

private:
    virtual void cancel_impl(ptr<delayed_task>& task) = 0;
};
} // namespace raft

#endif //_DELAYED_TASK_SCHEDULER_H_