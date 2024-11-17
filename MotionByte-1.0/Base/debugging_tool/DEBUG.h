#pragma once
#define DEBUG 1
#define DEBUG 6
// #define RELEASE
#define FPS_SHOW true
#include <string>
#include <mutex>
#include <iostream>
#include <sstream>
#include <utility>
// Variadic template function to handle argument unpacking
template <typename T>
void appendToStream(std::ostringstream& oss, T&& arg) {
    oss << std::forward<T>(arg);
}

template <typename T, typename... Args>
void appendToStream(std::ostringstream& oss, T&& first, Args&&... rest) {
    oss << std::forward<T>(first); // Add the first argument
    appendToStream(oss, std::forward<Args>(rest)...); // Recurse for remaining arguments
}

// Debug function with variadic templates
template <typename... Args>
void debug(int level, Args&&... args) {
    if (level > DEBUG) {
        return;
    }
    std::ostringstream oss;
    // Append all arguments to the stream
    appendToStream(oss, std::forward<Args>(args)...);

    // Output the final debug message
    std::cout << oss.str() << std::endl;
}