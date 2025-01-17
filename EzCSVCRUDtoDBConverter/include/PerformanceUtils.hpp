// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com
#ifndef PERFORMANCEUTILS_HPP
#define PERFORMANCEUTILS_HPP

#include <iostream>
#include <functional>
#include <chrono>
#include <iomanip>

namespace EzCSCCRUDtoDBConverter {
	// Template declaration
	template <typename T>
	T measurePerformance(const std::string& operationName, const std::function<T()>& operation);

	// Overload for operations that do not return a value (void)
	void measurePerformance(const std::string& operationName, const std::function<void()>& operation);
}
// This tells the compiler where to find the implementation of the template functions
#include "../src/PerformanceUtils.tpp"

#endif


