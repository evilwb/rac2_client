#pragma once

#include <cstdarg>
#include <cstddef>
#include <cstdio>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

inline const std::vector<std::string> tokenize(const std::string text, const char delim = ' ') {
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream token_stream(text);
    while (std::getline(token_stream, token, delim)) {
        tokens.push_back(token);
    }
    return tokens;
}

inline int printp(const char * format, ...) {
    va_list args;
    va_start(args, format);
    
    printf("\n");
    int result = vprintf(format, args);
    printf("\n> ");
    std::cout << std::flush;
    return result;
}