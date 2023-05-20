#pragma once
#include <queue>
#include <thread>
#include <unordered_set>
#include <condition_variable>
#include <future>
#include "api.hpp"

namespace app
{
    class config;
}

namespace app::core
{
    class thread_pool
    {
    public:
        explicit thread_pool(const app::config& config);
        ~thread_pool();
        
        void run();
        void wait(api::task_id_t task_id);
        void wait_all();
        bool is_task_done(api::task_id_t task_id);

        template <typename func, typename ...Args>
        api::task_id_t add_task(const func& task, Args&&... args) 
        {
            api::task_id_t task_id = last_id++;

            std::lock_guard<std::mutex> _(que_mtx);
            que.emplace(std::async(std::launch::deferred, task, args...), task_id);
            
            que_cv.notify_one();
            return task_id;
        }

    private:
        std::queue<std::pair<std::future<void>, api::task_id_t>> que; 
        std::condition_variable que_cv;

        std::unordered_set<api::task_id_t> complete_tasks; 
        std::condition_variable complete_tasks_cv;

        std::vector<std::thread> _threads;

        std::atomic<bool> end{false};

        std::atomic<api::task_id_t> last_id = 0;

        std::mutex que_mtx;
        std::mutex complete_tasks_mtx;
    };
}
