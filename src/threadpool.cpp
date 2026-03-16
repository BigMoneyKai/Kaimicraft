#include "threadpool.h"

void ThreadPool::init() {
    m_corenum = std::thread::hardware_concurrency() - 1;
    if(m_corenum == 0) {
        FATAL("Unable to detect cpu cores");
    } else {
        INFO("Detected available cpu cores: %d", m_corenum);
    }
    m_quit = false;

    for(size_t i = 0; i < m_corenum; i++) {
        m_workers.emplace_back([this] {
            while(true) {
                std::function<void()> task;
                {
                    std::unique_lock<std::mutex> lock(m_queueMutex);
                    m_cv.wait(lock, [this] {
                        return m_quit || !m_tasks.empty();
                    });
                    if(m_quit && m_tasks.empty()) return;

                    task = std::move(m_tasks.front());
                    m_tasks.pop_front();
                }
                if(task) task();
                else WARNING("Empty function detected");
            }
        });
    }
}

void ThreadPool::enqueueTask(std::function<void()> task) {
    {
        std::unique_lock<std::mutex> lock(m_queueMutex);
        m_tasks.push_back(std::move(task));
    }
    m_cv.notify_one();
}

void ThreadPool::destroy() {
    {
        std::unique_lock<std::mutex> lock(m_queueMutex);
        m_quit = true;
    }
    m_cv.notify_all();
    for(std::thread& worker: m_workers)
        if(worker.joinable()) worker.join();
}
