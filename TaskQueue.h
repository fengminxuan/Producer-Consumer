#pragma once
#include<vector>
#include<thread>
#include<functional>
#include<queue>
#include<mutex>

class TaskQueue
{
public:
    TaskQueue(int threadCount = 1);
    ~TaskQueue();
    void Add(std::function<void(void)> func);
private:
    std::mutex m_metex;
    std::condition_variable m_cv;
    std::vector<std::thread> m_workers;
    std::queue<std::function<void(void)>> m_tasks;

    void Worker();
};