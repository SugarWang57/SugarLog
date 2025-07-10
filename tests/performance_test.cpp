/**
 * @file performance_test.cpp
 * @brief SugarLog 性能测试
 */

#include "sugarlog.hpp"
#include <iostream>
#include <chrono>
#include <thread>
#include <vector>

using namespace sugarlog;

int main() {
    std::cout << "=== SugarLog 性能测试 ===" << std::endl;
    
    // 初始化日志系统
    initialize(LogConfig{
        .level = LogLevel::INFO,
        .async = true,
        .colored = false,
        .pattern = "[%Y-%m-%d %H:%M:%S.%3N] [%l] [%t] %v",
        .show_milliseconds = true,
        .show_filename = false,
        .show_function = false,
        .show_thread_id = true,
        .queue_size = 100000,
        .batch_size = 1000,
        .flush_interval_ms = 100,
        .worker_threads = 4,
        .enable_performance_monitoring = true
    });
    
    const int iterations = 100000;
    const int thread_count = 8;
    
    std::cout << "\n测试配置:" << std::endl;
    std::cout << "  迭代次数: " << iterations << std::endl;
    std::cout << "  线程数: " << thread_count << std::endl;
    std::cout << "  总日志数: " << (iterations * thread_count) << std::endl;
    
    // 单线程性能测试
    std::cout << "\n1. 单线程性能测试..." << std::endl;
    {
        SUGARLOG_PERF_SCOPE("single_thread_test");
        
        auto start = std::chrono::high_resolution_clock::now();
        
        for (int i = 0; i < iterations; ++i) {
            SUGARLOG_INFO("单线程测试消息 #" + std::to_string(i));
        }
        
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        
        std::cout << "   完成时间: " << duration.count() << " ms" << std::endl;
        std::cout << "   平均每条日志: " << (duration.count() * 1000000.0 / iterations) << " ns" << std::endl;
        std::cout << "   吞吐量: " << (iterations * 1000.0 / duration.count()) << " logs/sec" << std::endl;
    }
    
    // 多线程性能测试
    std::cout << "\n2. 多线程性能测试..." << std::endl;
    {
        SUGARLOG_PERF_SCOPE("multi_thread_test");
        
        auto start = std::chrono::high_resolution_clock::now();
        
        std::vector<std::thread> threads;
        for (int t = 0; t < thread_count; ++t) {
            threads.emplace_back([t, iterations]() {
                for (int i = 0; i < iterations; ++i) {
                    SUGARLOG_INFO("线程 " + std::to_string(t) + " 消息 #" + std::to_string(i));
                }
            });
        }
        
        for (auto& thread : threads) {
            thread.join();
        }
        
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        
        std::cout << "   完成时间: " << duration.count() << " ms" << std::endl;
        std::cout << "   平均每条日志: " << (duration.count() * 1000000.0 / (iterations * thread_count)) << " ns" << std::endl;
        std::cout << "   吞吐量: " << ((iterations * thread_count) * 1000.0 / duration.count()) << " logs/sec" << std::endl;
    }
    
    // 不同日志级别测试
    std::cout << "\n3. 不同日志级别测试..." << std::endl;
    {
        SUGARLOG_PERF_SCOPE("log_levels_test");
        
        auto start = std::chrono::high_resolution_clock::now();
        
        for (int i = 0; i < iterations / 10; ++i) {
            SUGARLOG_TRACE("TRACE消息 #" + std::to_string(i));
            SUGARLOG_DEBUG("DEBUG消息 #" + std::to_string(i));
            SUGARLOG_INFO("INFO消息 #" + std::to_string(i));
            SUGARLOG_WARN("WARN消息 #" + std::to_string(i));
            SUGARLOG_ERROR("ERROR消息 #" + std::to_string(i));
            SUGARLOG_FATAL("FATAL消息 #" + std::to_string(i));
        }
        
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        
        std::cout << "   完成时间: " << duration.count() << " ms" << std::endl;
        std::cout << "   平均每条日志: " << (duration.count() * 1000000.0 / (iterations * 6 / 10)) << " ns" << std::endl;
    }
    
    // 结构化日志测试
    std::cout << "\n4. 结构化日志测试..." << std::endl;
    {
        SUGARLOG_PERF_SCOPE("structured_logging_test");
        
        auto start = std::chrono::high_resolution_clock::now();
        
        for (int i = 0; i < iterations / 10; ++i) {
            Logger::get().log_structured(LogLevel::INFO, "user_action", {
                {"user_id", std::to_string(i)},
                {"action", "login"},
                {"ip", "192.168.1." + std::to_string(i % 255)},
                {"timestamp", std::to_string(i)}
            });
        }
        
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        
        std::cout << "   完成时间: " << duration.count() << " ms" << std::endl;
        std::cout << "   平均每条日志: " << (duration.count() * 1000000.0 / (iterations / 10)) << " ns" << std::endl;
    }
    
    // 等待所有日志处理完成
    std::cout << "\n5. 等待日志处理完成..." << std::endl;
    flush();
    
    // 获取性能统计
    std::cout << "\n6. 性能统计信息:" << std::endl;
    auto stats = get_performance_stats();
    std::cout << stats << std::endl;
    
    // 获取日志管理器统计
    std::cout << "\n7. 日志管理器统计:" << std::endl;
    auto manager_stats = LogManager::get().get_stats();
    std::cout << "日志管理器统计信息:" << std::endl;
    std::cout << manager_stats << std::endl;
    
    std::cout << "   队列大小: " << manager_stats << std::endl;
    std::cout << "   队列容量: " << manager_stats << std::endl;
    std::cout << "   丢弃消息数: " << manager_stats << std::endl;
    std::cout << "   丢弃率: " << manager_stats << "%" << std::endl;
    
    // 关闭日志系统
    std::cout << "\n8. 关闭日志系统..." << std::endl;
    shutdown();
    
    std::cout << "\n=== 性能测试完成 ===" << std::endl;
    return 0;
} 