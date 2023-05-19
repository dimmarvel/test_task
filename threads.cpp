#include "threads.hpp"
#include "config.hpp"

namespace app::core
{
    thread_pool::thread_pool(const app::config& config)
    {
        uint32_t t_num = config.get_threads_amount();
        _threads.reserve(t_num);
        for (uint32_t i = 0; i < t_num; ++i)
            _threads.emplace_back(&thread_pool::run, this);
    }

    thread_pool::~thread_pool() 
    {
        wait_all();
        end = true;
        for (uint32_t i = 0; i < _threads.size(); ++i) 
        {
            que_cv.notify_all();
            _threads.at(i).join();
        }
    }

    void thread_pool::run()
    {
        while (!end) {
            std::unique_lock<std::mutex> u_lock(que_mtx);
            
            que_cv.wait(u_lock, [this]() -> bool 
                { 
                    return !que.empty() || end; 
                });

            if (!que.empty()) {
                
                auto task = std::move(que.front());
                que.pop();
                u_lock.unlock();

                task.first.get();

                std::lock_guard<std::mutex> _(complete_tasks_mtx);
                
                complete_tasks.insert(task.second);
                complete_tasks_cv.notify_all();
            }
        }
    }

    void thread_pool::wait(api::task_id_t task_id) 
    {
        std::unique_lock<std::mutex> lock(complete_tasks_mtx);
        
        complete_tasks_cv.wait(lock, [this, task_id]() -> bool 
            {
                return complete_tasks.find(task_id) != complete_tasks.end(); 
            });
    }

    void thread_pool::wait_all() 
    {
        std::unique_lock<std::mutex> lock(que_mtx);
        
        complete_tasks_cv.wait(lock, [this]() -> bool 
            {
                std::lock_guard<std::mutex> task_lock(complete_tasks_mtx);
                return que.empty() && last_id == complete_tasks.size();
            });
    }

    bool thread_pool::is_task_done(api::task_id_t task_id) 
    {
        std::lock_guard<std::mutex> lock(complete_tasks_mtx);
        if (complete_tasks.find(task_id) != complete_tasks.end())
            return true;
        return false;
    }
}
