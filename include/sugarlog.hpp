/**
 * @file sugarlog.hpp
 * @brief SugarLog - 高性能现代C++日志库
 * @author Sugar
 * @version 1.0.0
 * 
 * 特性:
 * - 高性能异步日志写入
 * - 线程安全设计
 * - 内存池优化
 * - 多种输出格式
 * - 完整的异常安全保证
 */

#pragma once

#include "log_level.hpp"
#include "logger.hpp"
#include "log_manager.hpp"
#include "performance_monitor.hpp"

namespace sugarlog {

/**
 * @brief 快速日志宏定义
 */
#define SUGARLOG_TRACE(...)    sugarlog::Logger::get().trace(__VA_ARGS__)
#define SUGARLOG_DEBUG(...)    sugarlog::Logger::get().debug(__VA_ARGS__)
#define SUGARLOG_INFO(...)     sugarlog::Logger::get().info(__VA_ARGS__)
#define SUGARLOG_WARN(...)     sugarlog::Logger::get().warn(__VA_ARGS__)
#define SUGARLOG_ERROR(...)    sugarlog::Logger::get().error(__VA_ARGS__)
#define SUGARLOG_FATAL(...)    sugarlog::Logger::get().fatal(__VA_ARGS__)

/**
 * @brief 条件日志宏
 */
#define SUGARLOG_IF(condition, level, ...) \
    if (condition) { sugarlog::Logger::get().log(level, __VA_ARGS__); }

/**
 * @brief 性能监控宏
 */
#define SUGARLOG_PERF_START(name) \
    sugarlog::PerformanceMonitor::get().start(name)

#define SUGARLOG_PERF_END(name) \
    sugarlog::PerformanceMonitor::get().end(name)

#define SUGARLOG_PERF_SCOPE(name) \
    sugarlog::PerformanceScope scope(name)

/**
 * @brief 初始化日志系统
 * @param config 配置参数
 */
inline void initialize(const LogConfig& config = LogConfig{}) {
    LogManager::get().initialize(config);
}

/**
 * @brief 关闭日志系统
 */
inline void shutdown() {
    LogManager::get().shutdown();
}

/**
 * @brief 设置日志级别
 * @param level 日志级别
 */
inline void set_level(LogLevel level) {
    LogManager::get().set_level(level);
}

/**
 * @brief 获取当前日志级别
 * @return 当前日志级别
 */
inline LogLevel get_level() {
    return LogManager::get().get_level();
}

/**
 * @brief 添加日志输出目标
 * @param sink 日志输出目标
 */
inline void add_sink(std::shared_ptr<LogSink> sink) {
    LogManager::get().add_sink(sink);
}

/**
 * @brief 移除日志输出目标
 * @param sink 日志输出目标
 */
inline void remove_sink(std::shared_ptr<LogSink> sink) {
    LogManager::get().remove_sink(sink);
}

/**
 * @brief 清空所有日志输出目标
 */
inline void clear_sinks() {
    LogManager::get().clear_sinks();
}

/**
 * @brief 强制刷新所有日志输出
 */
inline void flush() {
    LogManager::get().flush();
}

/**
 * @brief 获取性能统计信息
 * @return 性能统计信息
 */
inline PerformanceStats get_performance_stats() {
    return PerformanceMonitor::get().get_stats("default");
}

} // namespace sugarlog 