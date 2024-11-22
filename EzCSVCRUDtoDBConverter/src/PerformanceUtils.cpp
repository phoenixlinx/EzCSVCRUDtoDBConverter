// This is a personal academic project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com
#include "../include/PerformanceUtils.hpp"

void measurePerformance(const std::string& callableName, const std::function<void()>& callable) {
    using Clock = std::chrono::high_resolution_clock;

    auto start = Clock::now();
    callable();
    auto end = Clock::now();
    std::chrono::duration<double> elapsedSeconds = end - start;
    std::cout << callableName << " - Time: " << elapsedSeconds.count() << " seconds" << std::endl << std::endl;
}
