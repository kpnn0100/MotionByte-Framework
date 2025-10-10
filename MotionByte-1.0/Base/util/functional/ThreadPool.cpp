#include "ThreadPool.h"

namespace MotionByte
{
    ThreadPool::ThreadPool(size_t threads) 
        : numThreads(threads), stop(false)
    {
        // Ensure at least one thread
        if (numThreads == 0) {
            numThreads = 1;
        }
        
        // Create worker threads
        for(size_t i = 0; i < numThreads; ++i) {
            workers.emplace_back([this] {
                // Worker thread main loop
                for(;;) {
                    function<void()> task;
                    
                    {
                        unique_lock<mutex> lock(this->queueMutex);
                        
                        // Wait for a task or stop signal
                        this->condition.wait(lock, [this]{ 
                            return this->stop || !this->tasks.empty(); 
                        });
                        
                        // If stopping and no tasks left, exit
                        if(this->stop && this->tasks.empty())
                            return;
                        
                        // Get next task
                        task = move(this->tasks.front());
                        this->tasks.pop();
                    }
                    
                    // Execute the task
                    try {
                        task();
                    } catch(...) {
                        // Swallow exceptions to prevent thread termination
                        // In a production environment, you might want to log this
                    }
                }
            });
        }
    }
    
    ThreadPool::~ThreadPool()
    {
        {
            unique_lock<mutex> lock(queueMutex);
            stop = true;
        }
        
        // Notify all threads to wake up and check stop condition
        condition.notify_all();
        
        // Wait for all threads to finish
        for(thread &worker: workers) {
            if(worker.joinable()) {
                worker.join();
            }
        }
    }
    
    size_t ThreadPool::getThreadCount() const
    {
        return numThreads;
    }
    
    size_t ThreadPool::getPendingTaskCount() const
    {
        unique_lock<mutex> lock(queueMutex);
        return tasks.size();
    }
    
    bool ThreadPool::isStopping() const
    {
        return stop.load();
    }
    
    void ThreadPool::waitForAll()
    {
        // Wait until all tasks are completed
        unique_lock<mutex> lock(queueMutex);
        condition.wait(lock, [this]{ 
            return this->tasks.empty(); 
        });
    }
}