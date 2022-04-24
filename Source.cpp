#include "TaskQueue.h"
#include<iostream>
#include <chrono>

int main()
{
    auto t_start = std::chrono::high_resolution_clock::now();
    {
        TaskQueue tq(5);
        std::mutex metex;
        for (int i = 0; i < 16; i++)
        {
            tq.Add([&metex]
                {
                    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
                    std::lock_guard<std::mutex> guard(metex);
                    std::cout << "Sleep 1000" << std::endl;
                });
        }
    }
    auto t_end = std::chrono::high_resolution_clock::now();
    double elapsed_time_ms = std::chrono::duration<double, std::milli>(t_end - t_start).count();
    std::cout << "spend " << elapsed_time_ms << " ms" << std::endl;

    int a;
    std::cin >> a;
}
