#include "TaskQueue.h"

TaskQueue::TaskQueue(int threadCount) : m_workers()
{
    for (int i = 0; i < threadCount; i++)
    {
        m_workers.push_back(std::thread(&TaskQueue::Worker, this));
    }
}

TaskQueue::~TaskQueue()
{
    std::function<void(void)> emptyFunc;
    for (int i = 0; i < m_workers.size(); i++)
    {
        std::lock_guard<std::mutex> guard(m_metex);
        m_tasks.push(emptyFunc);
        m_cv.notify_one();
    }
    for (int i = 0; i < m_workers.size(); i++)
    {
        if (m_workers[i].joinable())
            m_workers[i].join();
    }
}

void TaskQueue::Add(std::function<void(void)> func)
{
    if (func)
    {
        std::lock_guard<std::mutex> guard(m_metex);
        m_tasks.push(func);
        m_cv.notify_one();
    }
}

void TaskQueue::Worker()
{
    while (true)
    {
        std::function<void(void)> func;
        {
            std::unique_lock<std::mutex> lock(m_metex);
            m_cv.wait(lock, [this]() { return !m_tasks.empty(); });
            func = m_tasks.front();
            m_tasks.pop();
        }
        if (func)
            func();
        else
            break;
    }
}