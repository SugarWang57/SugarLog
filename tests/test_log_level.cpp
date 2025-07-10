/**
 * @file test_log_level.cpp
 * @brief 日志级别单元测试
 */

#include "log_level.hpp"
#include <iostream>
#include <cassert>
#include <sstream> // Added for std::ostringstream

using namespace sugarlog;

void test_log_level_conversion() {
    std::cout << "Testing log level conversion..." << std::endl;
    
    // Test to_string
    assert(to_string(LogLevel::TRACE) == "TRACE");
    assert(to_string(LogLevel::DEBUG) == "DEBUG");
    assert(to_string(LogLevel::INFO) == "INFO");
    assert(to_string(LogLevel::WARN) == "WARN");
    assert(to_string(LogLevel::ERROR) == "ERROR");
    assert(to_string(LogLevel::FATAL) == "FATAL");
    
    // Test from_string
    assert(from_string("TRACE") == LogLevel::TRACE);
    assert(from_string("trace") == LogLevel::TRACE);
    assert(from_string("DEBUG") == LogLevel::DEBUG);
    assert(from_string("debug") == LogLevel::DEBUG);
    assert(from_string("INFO") == LogLevel::INFO);
    assert(from_string("info") == LogLevel::INFO);
    assert(from_string("WARN") == LogLevel::WARN);
    assert(from_string("warn") == LogLevel::WARN);
    assert(from_string("ERROR") == LogLevel::ERROR);
    assert(from_string("error") == LogLevel::ERROR);
    assert(from_string("FATAL") == LogLevel::FATAL);
    assert(from_string("fatal") == LogLevel::FATAL);
    
    // Test to_color_string
    assert(!get_color_code(LogLevel::INFO).empty());
    
    // Test to_short_string
    assert(to_short_string(LogLevel::TRACE) == "T");
    assert(to_short_string(LogLevel::DEBUG) == "D");
    assert(to_short_string(LogLevel::INFO) == "I");
    assert(to_short_string(LogLevel::WARN) == "W");
    assert(to_short_string(LogLevel::ERROR) == "E");
    assert(to_short_string(LogLevel::FATAL) == "F");
    
    std::cout << "  ✓ Log level conversion tests passed" << std::endl;
}

void test_log_level_comparison() {
    std::cout << "Testing log level comparison..." << std::endl;
    
    // Test comparison operators
    assert(LogLevel::TRACE < LogLevel::DEBUG);
    assert(LogLevel::DEBUG < LogLevel::INFO);
    assert(LogLevel::INFO < LogLevel::WARN);
    assert(LogLevel::WARN < LogLevel::ERROR);
    assert(LogLevel::ERROR < LogLevel::FATAL);
    
    assert(LogLevel::FATAL > LogLevel::ERROR);
    assert(LogLevel::ERROR > LogLevel::WARN);
    assert(LogLevel::WARN > LogLevel::INFO);
    assert(LogLevel::INFO > LogLevel::DEBUG);
    assert(LogLevel::DEBUG > LogLevel::TRACE);
    
    assert(LogLevel::INFO <= LogLevel::INFO);
    assert(LogLevel::INFO >= LogLevel::INFO);
    
    std::cout << "  ✓ Log level comparison tests passed" << std::endl;
}

void test_log_level_stream_operator() {
    std::cout << "Testing log level stream operator..." << std::endl;
    
    std::ostringstream oss;
    oss << LogLevel::INFO;
    assert(oss.str() == "INFO");
    
    std::cout << "  ✓ Log level stream operator tests passed" << std::endl;
}

void test_log_level_exceptions() {
    std::cout << "Testing log level exceptions..." << std::endl;
    
    bool exception_thrown = false;
    try {
        std::cout << "About to call from_string('INVALID')..." << std::endl;
        try {
            from_string("INVALID");
            std::cout << "No exception thrown for INVALID input!" << std::endl;
        } catch (const std::invalid_argument& e) {
            exception_thrown = true;
            std::cout << "Caught exception: " << e.what() << std::endl;
            assert(std::string(e.what()).find("Invalid log level") != std::string::npos);
        }
        std::cout << "After inner try-catch, exception_thrown=" << exception_thrown << std::endl;
        if (!exception_thrown) {
            std::cout << "Exception was NOT thrown as expected!" << std::endl;
        }
        assert(exception_thrown);
        std::cout << "  ✓ Log level exception tests passed" << std::endl;
    } catch (const std::exception& e) {
        std::cout << "Outer catch: " << e.what() << std::endl;
        throw;
    } catch (...) {
        std::cout << "Outer catch: unknown exception" << std::endl;
        throw;
    }
}

int main() {
    std::cout << "=== Log Level Unit Tests ===" << std::endl;
    
    try {
        test_log_level_conversion();
        test_log_level_comparison();
        test_log_level_stream_operator();
        test_log_level_exceptions();
        
        std::cout << "\n=== All tests passed! ===" << std::endl;
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Test failed with exception: " << e.what() << std::endl;
        return 1;
    }
} 