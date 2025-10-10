#pragma once
#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <future>
#include <functional>
#include <stdexcept>
#include <atomic>

using namespace std;

namespace MotionByte
{
    class ThreadPool
    {
    private:
        // Thread management
        vector<thread> workers;
        queue<function<void()>> tasks;
        
        // Synchronization
        mutable mutex queueMutex;
        condition_variable condition;
        atomic<bool> stop;
        
        // Pool configuration
        size_t numThreads;
        
    public:
        // Constructor - creates thread pool with specified number of threads
        ThreadPool(size_t threads = thread::hardware_concurrency());
        
        // Destructor - safely shuts down all threads
        ~ThreadPool();
        
        // Submit a task to the thread pool
        template<class F, class... Args>
        auto enqueue(F&& f, Args&&... args) 
            -> future<typename result_of<F(Args...)>::type>;
        
        // Get the number of threads in the pool
        size_t getThreadCount() const;
        
        // Get the number of pending tasks
        size_t getPendingTaskCount() const;
        
        // Check if the thread pool is stopping
        bool isStopping() const;
        
        // Wait for all tasks to complete
        void waitForAll();
        
        // Disable copy constructor and assignment operator
        ThreadPool(const ThreadPool&) = delete;
        ThreadPool& operator=(const ThreadPool&) = delete;
    };
    
    // Template implementation for enqueue method
    template<class F, class... Args>
    auto ThreadPool::enqueue(F&& f, Args&&... args) 
        -> future<typename result_of<F(Args...)>::type>
    {
        using return_type = typename result_of<F(Args...)>::type;
        
        auto task = make_shared<packaged_task<return_type()>>(
            bind(forward<F>(f), forward<Args>(args)...)
        );
        
        future<return_type> result = task->get_future();
        
        {
            unique_lock<mutex> lock(queueMutex);
            
            // Don't allow enqueueing after stopping the pool
            if(stop)
                throw runtime_error("enqueue on stopped ThreadPool");
            
            tasks.emplace([task](){ (*task)(); });
        }
        condition.notify_one();
        return result;
    }
}