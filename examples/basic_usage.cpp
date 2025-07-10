/**
 * @file basic_usage.cpp
 * @brief SugarLog 基本使用示例
 */

#include "sugarlog.hpp"
#include "console_sink.hpp"
#include "file_sink.hpp"
#include <iostream>
#include <thread>
#include <chrono>

using namespace sugarlog;

int main() {
    std::cout << "=== SugarLog 基本使用示例 ===" << std::endl;
    
    // 1. 基本初始化
    std::cout << "\n1. 基本初始化..." << std::endl;
    initialize(LogConfig{
        .level = LogLevel::DEBUG,
        .async = true,
        .colored = true,
        .pattern = "[%Y-%m-%d %H:%M:%S.%3N] [%l] [%t] %v",
        .show_milliseconds = true,
        .show_filename = true,
        .show_function = true,
        .show_thread_id = true,
        .queue_size = 10000,
        .batch_size = 100,
        .flush_interval_ms = 1000,
        .worker_threads = 2,
        .enable_performance_monitoring = true
    });
    
    // 2. 基本日志记录
    std::cout << "\n2. 基本日志记录..." << std::endl;
    SUGARLOG_TRACE("这是一条TRACE日志");
    SUGARLOG_DEBUG("这是一条DEBUG日志");
    SUGARLOG_INFO("这是一条INFO日志");
    SUGARLOG_WARN("这是一条WARN日志");
    SUGARLOG_ERROR("这是一条ERROR日志");
    SUGARLOG_FATAL("这是一条FATAL日志");
    
    // 3. 条件日志
    std::cout << "\n3. 条件日志..." << std::endl;
    bool debug_mode = true;
    SUGARLOG_IF(debug_mode, LogLevel::DEBUG, "调试模式已启用");
    
    int user_count = 100;
    SUGARLOG_IF(user_count > 50, LogLevel::WARN, "用户数量过多: " + std::to_string(user_count));
    
    // 4. 性能监控
    std::cout << "\n4. 性能监控..." << std::endl;
    {
        SUGARLOG_PERF_SCOPE("数据处理");
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        
        // 模拟数据处理
        for (int i = 0; i < 1000; ++i) {
            // 模拟工作
        }
    }
    
    // 5. 多线程日志
    std::cout << "\n5. 多线程日志..." << std::endl;
    std::vector<std::thread> threads;
    for (int i = 0; i < 5; ++i) {
        threads.emplace_back([i]() {
            for (int j = 0; j < 10; ++j) {
                SUGARLOG_INFO("线程 " + std::to_string(i) + " 的第 " + std::to_string(j) + " 条日志");
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
            }
        });
    }
    
    for (auto& thread : threads) {
        thread.join();
    }
    
    // 6. 结构化日志
    std::cout << "\n6. 结构化日志..." << std::endl;
    Logger::get().log_structured(LogLevel::INFO, "user_login", {
        {"user_id", "12345"},
        {"ip", "192.168.1.100"},
        {"browser", "Chrome"},
        {"success", "true"}
    });
    
    // 7. 异常日志
    std::cout << "\n7. 异常日志..." << std::endl;
    try {
        throw std::runtime_error("模拟的运行时错误");
    } catch (const std::exception& e) {
        Logger::get().log_exception(LogLevel::ERROR, "处理用户请求时发生异常", e);
    }
    
    // 8. 性能日志
    std::cout << "\n8. 性能日志..." << std::endl;
    auto start = std::chrono::high_resolution_clock::now();
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
    
    Logger::get().log_performance(LogLevel::INFO, "数据库查询", 
                                 duration.count(), 1024);
    
    // 9. 审计日志
    std::cout << "\n9. 审计日志..." << std::endl;
    Logger::get().log_audit(LogLevel::INFO, "admin", "delete_user", 
                           "user:12345", "success", "用户已被删除");
    
    // 10. 安全日志
    std::cout << "\n10. 安全日志..." << std::endl;
    Logger::get().log_security(LogLevel::WARN, "failed_login", 
                              "192.168.1.200", "Firefox", "密码错误");
    
    // 11. 业务日志
    std::cout << "\n11. 业务日志..." << std::endl;
    Logger::get().log_business(LogLevel::INFO, "order_created", 
                              "ORDER_12345", "金额: 299.99, 商品: 手机");
    
    // 12. 调试日志
    std::cout << "\n12. 调试日志..." << std::endl;
    Logger::get().log_debug(LogLevel::DEBUG, "处理用户请求", {
        {"user_id", "12345"},
        {"request_id", "req_67890"},
        {"method", "POST"},
        {"path", "/api/users"}
    });
    
    // 13. 调用链日志
    std::cout << "\n13. 调用链日志..." << std::endl;
    Logger::get().log_trace(LogLevel::INFO, "trace_12345", "span_67890", 
                           "span_11111", "处理用户请求");
    
    // 14. 指标日志
    std::cout << "\n14. 指标日志..." << std::endl;
    Logger::get().log_metric(LogLevel::INFO, "request_count", 150.0, "requests/sec", {
        {"endpoint", "/api/users"},
        {"method", "GET"}
    });
    
    // 15. 添加自定义输出目标
    std::cout << "\n15. 添加自定义输出目标..." << std::endl;
    auto file_sink = std::make_shared<FileSink>(FileSinkConfig{
        .filename = "example.log",
        .rotation_policy = RotationPolicy::SIZE,
        .max_file_size = 1024 * 1024,  // 1MB
        .max_files = 5,
        .auto_flush = true
    });
    add_sink(file_sink);
    
    SUGARLOG_INFO("这条日志会同时输出到控制台和文件");
    
    // 16. 获取性能统计
    std::cout << "\n16. 获取性能统计..." << std::endl;
    auto stats = get_performance_stats();
    std::cout << "性能统计信息:" << std::endl;
    std::cout << stats << std::endl;
    
    // 17. 等待所有日志处理完成
    std::cout << "\n17. 等待日志处理完成..." << std::endl;
    flush();
    
    // 18. 关闭日志系统
    std::cout << "\n18. 关闭日志系统..." << std::endl;
    shutdown();
    
    std::cout << "\n=== 示例完成 ===" << std::endl;
    return 0;
} 