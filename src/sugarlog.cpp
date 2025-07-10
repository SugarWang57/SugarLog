/**
 * @file sugarlog.cpp
 * @brief SugarLog库的主要实现文件
 */

#include "sugarlog.hpp"

namespace sugarlog {

// 库初始化函数
void initialize_library() {
    // 初始化日志管理器
    LogManager::get();
    
    // 初始化性能监控器
    PerformanceMonitor::get();
}

// 库清理函数
void cleanup_library() {
    // 关闭日志管理器
    LogManager::get().shutdown();
    
    // 清理性能监控器
    PerformanceMonitor::get().reset_all_stats();
}

// 获取库版本信息
const char* get_version() {
    return "SugarLog v1.0.0";
}

// 获取库构建信息
const char* get_build_info() {
    return "Built with C++20, Threads support";
}

} // namespace sugarlog

// 库构造函数（可选）
__attribute__((constructor))
void sugarlog_init() {
    sugarlog::initialize_library();
}

// 库析构函数（可选）
__attribute__((destructor))
void sugarlog_cleanup() {
    sugarlog::cleanup_library();
} 