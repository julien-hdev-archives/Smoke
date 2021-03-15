#include<chrono>
#include<iostream>
#include<vector>

struct Timer
{
    std::vector<std::chrono::time_point<
                    std::chrono::high_resolution_clock,
                    std::chrono::nanoseconds
                    >> dates;
    Timer()
    {
        log();
    }
    void log()
    {
        std::chrono::time_point<
                    std::chrono::high_resolution_clock,
                    std::chrono::nanoseconds
                    >
            t = std::chrono::high_resolution_clock::now();
        dates.push_back(t);
    }

    void
    print()
    {
        for (size_t i = 1; i < dates.size(); ++i)
        {
            float delta = std::chrono::duration_cast<std::chrono::milliseconds>(dates[i] - dates[i-1]).count();
            std::cout << "Measure n°" << i << " :  " << delta << " ms"<< std::endl;
        }
    }
    
};
