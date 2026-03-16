#pragma once

#include <thread>
#include <atomic>
#include <deque>
#include <functional>
#include <vector>
#include <mutex>
#include <condition_variable>

#include "debug.h"

class ThreadPool {
public:
    void init();
    void enqueueTask(std::function<void()> task);
    void destroy();
private:
    std::vector<std::thread> m_workers;
    std::deque<std::function<void()>> m_tasks;
    std::mutex m_queueMutex;
    std::condition_variable m_cv;
    std::atomic<bool> m_quit;

    unsigned int m_corenum;
};
