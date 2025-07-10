/**
 * @file log_manager.hpp
 * @brief 日志管理器
 */

#pragma once

#include "log_level.hpp"
#include "log_sink.hpp"
#include "async_logger.hpp"
#include <memory>
#include <mutex>
#include <unordered_map>
#include <string>
#include <vector>
#include <algorithm>
#include <sstream>

namespace sugarlog {

/**
 * @brief 日志配置
 */
struct LogConfig {
    LogLevel level = LogLevel::INFO;              ///< 日志级别
    bool async = true;                            ///< 是否启用异步
    bool colored = false;                         ///< 是否启用颜色
    std::string pattern = "[%Y-%m-%d %H:%M:%S] [%l] [%t] %v"; ///< 格式字符串
    std::string time_format = "%Y-%m-%d %H:%M:%S"; ///< 时间格式
    bool show_milliseconds = false;               ///< 是否显示毫秒
    bool show_filename = true;                    ///< 是否显示文件名
    bool show_function = true;                    ///< 是否显示函数名
    bool show_thread_id = true;                   ///< 是否显示线程ID
    bool sensitive_filter = false;                ///< 是否启用敏感信息过滤
    size_t queue_size = 10000;                    ///< 队列大小
    size_t batch_size = 100;                      ///< 批量处理大小
    uint32_t flush_interval_ms = 1000;            ///< 刷新间隔
    size_t worker_threads = 1;                    ///< 工作线程数量
    bool auto_flush = true;                       ///< 是否自动刷新
    bool enable_performance_monitoring = false;   ///< 是否启用性能监控
};

/**
 * @brief 日志管理器
 * 全局日志管理，提供统一的日志接口
 */
class LogManager {
public:
    LogManager() = default;
    ~LogManager() = default;
    
    /**
     * @brief 获取单例实例
     * @return 日志管理器实例
     */
    static LogManager& get();
    
    /**
     * @brief 初始化日志系统
     * @param config 配置参数
     */
    void initialize(const LogConfig& config = LogConfig{});
    
    /**
     * @brief 关闭日志系统
     */
    void shutdown();
    
    /**
     * @brief 记录日志
     * @param level 日志级别
     * @param message 日志消息
     * @param file 文件名
     * @param line 行号
     * @param function 函数名
     */
    void log(LogLevel level, 
             const std::string& message,
             const std::string& file = "",
             uint32_t line = 0,
             const std::string& function = "");
    
    /**
     * @brief 记录TRACE级别日志
     * @param message 日志消息
     * @param file 文件名
     * @param line 行号
     * @param function 函数名
     */
    void trace(const std::string& message,
               const std::string& file = "",
               uint32_t line = 0,
               const std::string& function = "");
    
    /**
     * @brief 记录DEBUG级别日志
     * @param message 日志消息
     * @param file 文件名
     * @param line 行号
     * @param function 函数名
     */
    void debug(const std::string& message,
               const std::string& file = "",
               uint32_t line = 0,
               const std::string& function = "");
    
    /**
     * @brief 记录INFO级别日志
     * @param message 日志消息
     * @param file 文件名
     * @param line 行号
     * @param function 函数名
     */
    void info(const std::string& message,
              const std::string& file = "",
              uint32_t line = 0,
              const std::string& function = "");
    
    /**
     * @brief 记录WARN级别日志
     * @param message 日志消息
     * @param file 文件名
     * @param line 行号
     * @param function 函数名
     */
    void warn(const std::string& message,
              const std::string& file = "",
              uint32_t line = 0,
              const std::string& function = "");
    
    /**
     * @brief 记录ERROR级别日志
     * @param message 日志消息
     * @param file 文件名
     * @param line 行号
     * @param function 函数名
     */
    void error(const std::string& message,
               const std::string& file = "",
               uint32_t line = 0,
               const std::string& function = "");
    
    /**
     * @brief 记录FATAL级别日志
     * @param message 日志消息
     * @param file 文件名
     * @param line 行号
     * @param function 函数名
     */
    void fatal(const std::string& message,
               const std::string& file = "",
               uint32_t line = 0,
               const std::string& function = "");
    
    /**
     * @brief 设置日志级别
     * @param level 日志级别
     */
    void set_level(LogLevel level);
    
    /**
     * @brief 获取日志级别
     * @return 日志级别
     */
    LogLevel get_level() const;
    
    /**
     * @brief 检查是否应该输出指定级别的日志
     * @param level 日志级别
     * @return 是否应该输出
     */
    bool should_log(LogLevel level) const;
    
    /**
     * @brief 添加日志输出目标
     * @param sink 日志输出目标
     */
    void add_sink(std::shared_ptr<LogSink> sink);
    
    /**
     * @brief 移除日志输出目标
     * @param sink 日志输出目标
     */
    void remove_sink(std::shared_ptr<LogSink> sink);
    
    /**
     * @brief 清空所有日志输出目标
     */
    void clear_sinks();
    
    /**
     * @brief 强制刷新所有日志
     */
    void flush();
    
    /**
     * @brief 获取配置
     * @return 配置参数
     */
    const LogConfig& get_config() const;
    
    /**
     * @brief 更新配置
     * @param config 新的配置参数
     */
    void update_config(const LogConfig& config);
    
    /**
     * @brief 获取统计信息
     * @return 统计信息字符串
     */
    std::string get_stats() const;
    
    /**
     * @brief 重置统计信息
     */
    void reset_stats();
    
    /**
     * @brief 创建命名日志器
     * @param name 日志器名称
     * @return 日志器引用
     */
    LogManager& create_logger(const std::string& name);
    
    /**
     * @brief 获取命名日志器
     * @param name 日志器名称
     * @return 日志器引用
     */
    LogManager& get_logger(const std::string& name);
    
    /**
     * @brief 移除命名日志器
     * @param name 日志器名称
     */
    void remove_logger(const std::string& name);
    
    /**
     * @brief 检查日志器是否存在
     * @param name 日志器名称
     * @return 是否存在
     */
    bool has_logger(const std::string& name) const;
    
    /**
     * @brief 获取所有日志器名称
     * @return 日志器名称列表
     */
    std::vector<std::string> get_logger_names() const;
    
    /**
     * @brief 设置默认日志器
     * @param name 日志器名称
     */
    void set_default_logger(const std::string& name);
    
    /**
     * @brief 获取默认日志器
     * @return 默认日志器引用
     */
    LogManager& get_default_logger();
    
    /**
     * @brief 检查是否已初始化
     * @return 是否已初始化
     */
    bool is_initialized() const;
    
    /**
     * @brief 检查是否已关闭
     * @return 是否已关闭
     */
    bool is_shutdown() const;
    
private:
    
    LogConfig config_;                           ///< 配置参数
    std::unique_ptr<AsyncLogger> async_logger_;  ///< 异步日志器
    std::vector<std::shared_ptr<LogSink>> sinks_; ///< 日志输出目标列表
    std::unordered_map<std::string, std::unique_ptr<LogManager>> named_loggers_; ///< 命名日志器
    std::string default_logger_name_;            ///< 默认日志器名称
    std::atomic<bool> initialized_;              ///< 是否已初始化
    std::atomic<bool> shutdown_;                 ///< 是否已关闭
    mutable std::mutex mutex_;                   ///< 互斥锁
    
    /**
     * @brief 创建异步日志器
     */
    void create_async_logger();
    
    /**
     * @brief 销毁异步日志器
     */
    void destroy_async_logger();
    
    /**
     * @brief 应用配置到所有输出目标
     */
    void apply_config_to_sinks();
    
    /**
     * @brief 创建默认输出目标
     */
    void create_default_sinks();
    
    // 禁用拷贝和赋值
    LogManager(const LogManager&) = delete;
    LogManager& operator=(const LogManager&) = delete;
};

// 单例实现
inline LogManager& LogManager::get() {
    static LogManager instance;
    return instance;
}

// 成员函数实现
inline void LogManager::initialize(const LogConfig& config) {
    std::lock_guard<std::mutex> lock(mutex_);
    if (initialized_) {
        return;
    }
    
    config_ = config;
    
    if (config.async) {
        AsyncLoggerConfig async_config;
        async_config.queue_size = config.queue_size;
        async_config.batch_size = config.batch_size;
        async_config.flush_interval_ms = config.flush_interval_ms;
        async_config.worker_threads = config.worker_threads;
        async_config.auto_flush = config.auto_flush;
        
        async_logger_ = std::make_unique<AsyncLogger>(async_config);
        async_logger_->start();
    }
    
    initialized_ = true;
    shutdown_ = false;
}

inline void LogManager::shutdown() {
    std::lock_guard<std::mutex> lock(mutex_);
    if (!initialized_ || shutdown_) {
        return;
    }
    
    if (async_logger_) {
        async_logger_->stop(true);
        async_logger_.reset();
    }
    
    flush();
    shutdown_ = true;
}

inline void LogManager::log(LogLevel level, 
                           const std::string& message,
                           const std::string& file,
                           uint32_t line,
                           const std::string& function) {
    if (!should_log(level)) {
        return;
    }
    
    LogMessage log_msg(level, message, file, line, function);
    
    if (async_logger_) {
        async_logger_->log(std::move(log_msg));
    } else {
        // 同步输出
        for (auto& sink : sinks_) {
            if (sink->should_log(level)) {
                sink->log(log_msg);
            }
        }
    }
}

inline void LogManager::trace(const std::string& message,
                             const std::string& file,
                             uint32_t line,
                             const std::string& function) {
    log(LogLevel::TRACE, message, file, line, function);
}

inline void LogManager::debug(const std::string& message,
                             const std::string& file,
                             uint32_t line,
                             const std::string& function) {
    log(LogLevel::DEBUG, message, file, line, function);
}

inline void LogManager::info(const std::string& message,
                            const std::string& file,
                            uint32_t line,
                            const std::string& function) {
    log(LogLevel::INFO, message, file, line, function);
}

inline void LogManager::warn(const std::string& message,
                            const std::string& file,
                            uint32_t line,
                            const std::string& function) {
    log(LogLevel::WARN, message, file, line, function);
}

inline void LogManager::error(const std::string& message,
                             const std::string& file,
                             uint32_t line,
                             const std::string& function) {
    log(LogLevel::ERROR, message, file, line, function);
}

inline void LogManager::fatal(const std::string& message,
                             const std::string& file,
                             uint32_t line,
                             const std::string& function) {
    log(LogLevel::FATAL, message, file, line, function);
}

inline void LogManager::set_level(LogLevel level) {
    std::lock_guard<std::mutex> lock(mutex_);
    config_.level = level;
}

inline LogLevel LogManager::get_level() const {
    std::lock_guard<std::mutex> lock(mutex_);
    return config_.level;
}

inline bool LogManager::should_log(LogLevel level) const {
    std::lock_guard<std::mutex> lock(mutex_);
    return level >= config_.level;
}

inline void LogManager::add_sink(std::shared_ptr<LogSink> sink) {
    std::lock_guard<std::mutex> lock(mutex_);
    sinks_.push_back(sink);
}

inline void LogManager::remove_sink(std::shared_ptr<LogSink> sink) {
    std::lock_guard<std::mutex> lock(mutex_);
    auto it = std::find(sinks_.begin(), sinks_.end(), sink);
    if (it != sinks_.end()) {
        sinks_.erase(it);
    }
}

inline void LogManager::clear_sinks() {
    std::lock_guard<std::mutex> lock(mutex_);
    sinks_.clear();
}

inline void LogManager::flush() {
    std::lock_guard<std::mutex> lock(mutex_);
    
    if (async_logger_) {
        async_logger_->flush();
    }
    
    for (auto& sink : sinks_) {
        sink->flush();
    }
}

inline const LogConfig& LogManager::get_config() const {
    std::lock_guard<std::mutex> lock(mutex_);
    return config_;
}

inline void LogManager::update_config(const LogConfig& config) {
    std::lock_guard<std::mutex> lock(mutex_);
    config_ = config;
}

inline std::string LogManager::get_stats() const {
    std::lock_guard<std::mutex> lock(mutex_);
    
    std::ostringstream oss;
    oss << "LogManager Stats:\n";
    oss << "  Initialized: " << (initialized_ ? "Yes" : "No") << "\n";
    oss << "  Shutdown: " << (shutdown_ ? "Yes" : "No") << "\n";
    oss << "  Level: " << to_string(config_.level) << "\n";
    oss << "  Async: " << (config_.async ? "Yes" : "No") << "\n";
    oss << "  Sinks: " << sinks_.size() << "\n";
    oss << "  Named Loggers: " << named_loggers_.size() << "\n";
    
    if (async_logger_) {
        oss << "  Queue Size: " << async_logger_->get_queue_size() << "\n";
        oss << "  Queue Capacity: " << async_logger_->get_queue_capacity() << "\n";
    }
    
    return oss.str();
}

inline void LogManager::reset_stats() {
    // 简单实现，实际可能需要重置更多统计信息
}

inline LogManager& LogManager::create_logger(const std::string& name) {
    std::lock_guard<std::mutex> lock(mutex_);
    
    auto logger = std::make_unique<LogManager>();
    logger->config_ = config_;
    named_loggers_[name] = std::move(logger);
    
    return *named_loggers_[name];
}

inline LogManager& LogManager::get_logger(const std::string& name) {
    std::lock_guard<std::mutex> lock(mutex_);
    
    auto it = named_loggers_.find(name);
    if (it != named_loggers_.end()) {
        return *it->second;
    }
    
    // 如果不存在，创建一个新的
    return create_logger(name);
}

inline void LogManager::remove_logger(const std::string& name) {
    std::lock_guard<std::mutex> lock(mutex_);
    named_loggers_.erase(name);
}

inline bool LogManager::has_logger(const std::string& name) const {
    std::lock_guard<std::mutex> lock(mutex_);
    return named_loggers_.find(name) != named_loggers_.end();
}

inline std::vector<std::string> LogManager::get_logger_names() const {
    std::lock_guard<std::mutex> lock(mutex_);
    
    std::vector<std::string> names;
    names.reserve(named_loggers_.size());
    
    for (const auto& pair : named_loggers_) {
        names.push_back(pair.first);
    }
    
    return names;
}

inline void LogManager::set_default_logger(const std::string& name) {
    std::lock_guard<std::mutex> lock(mutex_);
    default_logger_name_ = name;
}

inline LogManager& LogManager::get_default_logger() {
    return get_logger(default_logger_name_);
}

inline bool LogManager::is_initialized() const {
    std::lock_guard<std::mutex> lock(mutex_);
    return initialized_;
}

inline bool LogManager::is_shutdown() const {
    std::lock_guard<std::mutex> lock(mutex_);
    return shutdown_;
}

/**
 * @brief 日志管理器构建器
 * 提供流式接口来配置日志管理器
 */
class LogManagerBuilder {
public:
    /**
     * @brief 构造函数
     */
    LogManagerBuilder() = default;
    
    /**
     * @brief 设置日志级别
     * @param level 日志级别
     * @return 构建器引用
     */
    LogManagerBuilder& level(LogLevel level) {
        config_.level = level;
        return *this;
    }
    
    /**
     * @brief 设置是否启用异步
     * @param enabled 是否启用
     * @return 构建器引用
     */
    LogManagerBuilder& async(bool enabled) {
        config_.async = enabled;
        return *this;
    }
    
    /**
     * @brief 设置是否启用颜色
     * @param enabled 是否启用
     * @return 构建器引用
     */
    LogManagerBuilder& colored(bool enabled) {
        config_.colored = enabled;
        return *this;
    }
    
    /**
     * @brief 设置格式字符串
     * @param pattern 格式字符串
     * @return 构建器引用
     */
    LogManagerBuilder& pattern(const std::string& pattern) {
        config_.pattern = pattern;
        return *this;
    }
    
    /**
     * @brief 设置时间格式
     * @param format 时间格式
     * @return 构建器引用
     */
    LogManagerBuilder& time_format(const std::string& format) {
        config_.time_format = format;
        return *this;
    }
    
    /**
     * @brief 设置是否显示毫秒
     * @param enabled 是否显示
     * @return 构建器引用
     */
    LogManagerBuilder& show_milliseconds(bool enabled) {
        config_.show_milliseconds = enabled;
        return *this;
    }
    
    /**
     * @brief 设置是否显示文件名
     * @param enabled 是否显示
     * @return 构建器引用
     */
    LogManagerBuilder& show_filename(bool enabled) {
        config_.show_filename = enabled;
        return *this;
    }
    
    /**
     * @brief 设置是否显示函数名
     * @param enabled 是否显示
     * @return 构建器引用
     */
    LogManagerBuilder& show_function(bool enabled) {
        config_.show_function = enabled;
        return *this;
    }
    
    /**
     * @brief 设置是否显示线程ID
     * @param enabled 是否显示
     * @return 构建器引用
     */
    LogManagerBuilder& show_thread_id(bool enabled) {
        config_.show_thread_id = enabled;
        return *this;
    }
    
    /**
     * @brief 设置是否启用敏感信息过滤
     * @param enabled 是否启用
     * @return 构建器引用
     */
    LogManagerBuilder& sensitive_filter(bool enabled) {
        config_.sensitive_filter = enabled;
        return *this;
    }
    
    /**
     * @brief 设置队列大小
     * @param size 队列大小
     * @return 构建器引用
     */
    LogManagerBuilder& queue_size(size_t size) {
        config_.queue_size = size;
        return *this;
    }
    
    /**
     * @brief 设置批量处理大小
     * @param size 批量大小
     * @return 构建器引用
     */
    LogManagerBuilder& batch_size(size_t size) {
        config_.batch_size = size;
        return *this;
    }
    
    /**
     * @brief 设置刷新间隔
     * @param interval_ms 刷新间隔（毫秒）
     * @return 构建器引用
     */
    LogManagerBuilder& flush_interval(uint32_t interval_ms) {
        config_.flush_interval_ms = interval_ms;
        return *this;
    }
    
    /**
     * @brief 设置工作线程数量
     * @param count 线程数量
     * @return 构建器引用
     */
    LogManagerBuilder& worker_threads(size_t count) {
        config_.worker_threads = count;
        return *this;
    }
    
    /**
     * @brief 设置是否自动刷新
     * @param enabled 是否启用
     * @return 构建器引用
     */
    LogManagerBuilder& auto_flush(bool enabled) {
        config_.auto_flush = enabled;
        return *this;
    }
    
    /**
     * @brief 设置是否启用性能监控
     * @param enabled 是否启用
     * @return 构建器引用
     */
    LogManagerBuilder& enable_performance_monitoring(bool enabled) {
        config_.enable_performance_monitoring = enabled;
        return *this;
    }
    
    /**
     * @brief 构建并初始化日志管理器
     */
    void build() {
        LogManager::get().initialize(config_);
    }
    
private:
    LogConfig config_;
};

} // namespace sugarlog 