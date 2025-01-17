// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com
#ifndef PERFORMANCEUTILS_TPP
#define PERFORMANCEUTILS_TPP
#include <PerformanceUtils.hpp>


namespace EzCSCCRUDtoDBConverter {
    template <typename T>
    T measurePerformance(const std::string& callableName, const std::function<T()>& callable) {
        using Clock = std::chrono::high_resolution_clock;

        auto start = Clock::now();
        T result = callable();  // This gets the return value from the function/callable
        auto end = Clock::now();
        std::chrono::duration<double> elapsedSeconds = end - start;
        // The intent of fixed is to promote consistency of time output.
        std::cout << callableName << std::fixed << " - Time: " << elapsedSeconds.count() << " seconds" << std::endl << std::endl;

        return result;
    }
}
#endif