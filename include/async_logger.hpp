/**
 * @file async_logger.hpp
 * @brief 异步日志器
 */

#pragma once

#include "log_message.hpp"
#include "log_sink.hpp"
#include "thread_safe_queue.hpp"
#include "memory_pool.hpp"
#include <thread>
#include <atomic>
#include <chrono>
#include <vector>
#include <memory>

namespace sugarlog {

/**
 * @brief 异步日志器配置
 */
struct AsyncLoggerConfig {
    size_t queue_size = 10000;                    ///< 队列大小
    size_t batch_size = 100;                      ///< 批量处理大小
    uint32_t flush_interval_ms = 1000;            ///< 刷新间隔（毫秒）
    size_t worker_threads = 1;                    ///< 工作线程数量
    bool auto_flush = true;                       ///< 是否自动刷新
    size_t memory_pool_size = 1024;               ///< 内存池大小
    size_t max_memory_pool_blocks = 1000;         ///< 最大内存池块数
    bool enable_performance_monitoring = false;   ///< 是否启用性能监控
};

/**
 * @brief 异步日志器
 * 提供高性能的异步日志处理
 */
class AsyncLogger {
public:
    /**
     * @brief 构造函数
     * @param config 配置参数
     */
    explicit AsyncLogger(const AsyncLoggerConfig& config = AsyncLoggerConfig{});
    
    /**
     * @brief 析构函数
     */
    ~AsyncLogger();
    
    /**
     * @brief 禁用拷贝构造和赋值
     */
    AsyncLogger(const AsyncLogger&) = delete;
    AsyncLogger& operator=(const AsyncLogger&) = delete;
    
    /**
     * @brief 禁用移动构造和赋值
     */
    AsyncLogger(AsyncLogger&&) = delete;
    AsyncLogger& operator=(AsyncLogger&&) = delete;
    
    /**
     * @brief 启动异步日志器
     */
    void start();
    
    /**
     * @brief 停止异步日志器
     * @param wait_for_completion 是否等待所有日志处理完成
     */
    void stop(bool wait_for_completion = true);
    
    /**
     * @brief 添加日志消息
     * @param message 日志消息
     * @return 是否成功添加
     */
    bool log(LogMessage message);
    
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
     * @brief 获取队列大小
     * @return 队列大小
     */
    size_t get_queue_size() const;
    
    /**
     * @brief 获取队列容量
     * @return 队列容量
     */
    size_t get_queue_capacity() const;
    
    /**
     * @brief 获取丢弃的日志数量
     * @return 丢弃的日志数量
     */
    size_t get_dropped_count() const;
    
    /**
     * @brief 获取丢弃率
     * @return 丢弃率
     */
    double get_drop_rate() const;
    
    /**
     * @brief 检查是否正在运行
     * @return 是否正在运行
     */
    bool is_running() const;
    
    /**
     * @brief 获取工作线程数量
     * @return 工作线程数量
     */
    size_t get_worker_thread_count() const;
    
    /**
     * @brief 设置批量处理大小
     * @param batch_size 批量大小
     */
    void set_batch_size(size_t batch_size);
    
    /**
     * @brief 获取批量处理大小
     * @return 批量大小
     */
    size_t get_batch_size() const;
    
    /**
     * @brief 设置刷新间隔
     * @param interval_ms 刷新间隔（毫秒）
     */
    void set_flush_interval(uint32_t interval_ms);
    
    /**
     * @brief 获取刷新间隔
     * @return 刷新间隔（毫秒）
     */
    uint32_t get_flush_interval() const;
    
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
     * @brief 等待所有日志处理完成
     * @param timeout 超时时间
     * @return 是否成功等待
     */
    template<typename Rep, typename Period>
    bool wait_for_completion(const std::chrono::duration<Rep, Period>& timeout) {
        auto start_time = std::chrono::steady_clock::now();
        while (is_running() && !queue_.empty()) {
            auto elapsed = std::chrono::steady_clock::now() - start_time;
            if (elapsed >= timeout) {
                return false;
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
        return true;
    }
    
private:
    void worker_thread_func();
    void process_batch(const std::vector<LogMessage>& batch);
    void process_message(const LogMessage& msg);
    bool should_auto_flush();

    AsyncLoggerConfig config_;
    BatchQueue<LogMessage> queue_;
    std::vector<std::shared_ptr<LogSink>> sinks_;
    mutable std::mutex sinks_mutex_;
    std::vector<std::thread> workers_;
    std::atomic<bool> running_;
    std::atomic<bool> stop_requested_;
    std::chrono::steady_clock::time_point last_flush_time_;
    std::atomic<size_t> dropped_count_{0};
};

/**
 * @brief 异步日志器构建器
 * 提供流式接口来配置异步日志器
 */
class AsyncLoggerBuilder {
public:
    /**
     * @brief 构造函数
     */
    AsyncLoggerBuilder() = default;
    
    /**
     * @brief 设置队列大小
     * @param size 队列大小
     * @return 构建器引用
     */
    AsyncLoggerBuilder& queue_size(size_t size) {
        config_.queue_size = size;
        return *this;
    }
    
    /**
     * @brief 设置批量处理大小
     * @param size 批量大小
     * @return 构建器引用
     */
    AsyncLoggerBuilder& batch_size(size_t size) {
        config_.batch_size = size;
        return *this;
    }
    
    /**
     * @brief 设置刷新间隔
     * @param interval_ms 刷新间隔（毫秒）
     * @return 构建器引用
     */
    AsyncLoggerBuilder& flush_interval(uint32_t interval_ms) {
        config_.flush_interval_ms = interval_ms;
        return *this;
    }
    
    /**
     * @brief 设置工作线程数量
     * @param count 线程数量
     * @return 构建器引用
     */
    AsyncLoggerBuilder& worker_threads(size_t count) {
        config_.worker_threads = count;
        return *this;
    }
    
    /**
     * @brief 设置是否自动刷新
     * @param enabled 是否启用
     * @return 构建器引用
     */
    AsyncLoggerBuilder& auto_flush(bool enabled) {
        config_.auto_flush = enabled;
        return *this;
    }
    
    /**
     * @brief 设置内存池大小
     * @param size 内存池大小
     * @return 构建器引用
     */
    AsyncLoggerBuilder& memory_pool_size(size_t size) {
        config_.memory_pool_size = size;
        return *this;
    }
    
    /**
     * @brief 设置最大内存池块数
     * @param count 块数
     * @return 构建器引用
     */
    AsyncLoggerBuilder& max_memory_pool_blocks(size_t count) {
        config_.max_memory_pool_blocks = count;
        return *this;
    }
    
    /**
     * @brief 设置是否启用性能监控
     * @param enabled 是否启用
     * @return 构建器引用
     */
    AsyncLoggerBuilder& enable_performance_monitoring(bool enabled) {
        config_.enable_performance_monitoring = enabled;
        return *this;
    }
    
    /**
     * @brief 构建异步日志器
     * @return 异步日志器实例
     */
    AsyncLogger build() {
        return AsyncLogger(config_);
    }
    
private:
    AsyncLoggerConfig config_;
};

// AsyncLogger 实现
inline AsyncLogger::AsyncLogger(const AsyncLoggerConfig& config)
    : config_(config)
    , queue_(config.queue_size)
    , running_(false)
    , stop_requested_(false) {
}

inline AsyncLogger::~AsyncLogger() {
    stop(true);
}

inline void AsyncLogger::start() {
    if (running_) return;
    
    running_ = true;
    stop_requested_ = false;
    
    // 启动工作线程
    for (size_t i = 0; i < config_.worker_threads; ++i) {
        workers_.emplace_back([this]() {
            worker_thread_func();
        });
    }
}

inline void AsyncLogger::stop(bool wait_for_completion) {
    if (!running_) return;
    
    stop_requested_ = true;
    
    if (wait_for_completion) {
        // 等待所有工作线程完成
        for (auto& worker : workers_) {
            if (worker.joinable()) {
                worker.join();
            }
        }
    }
    
    workers_.clear();
    running_ = false;
}

inline bool AsyncLogger::log(LogMessage message) {
    if (!running_) return false;
    
    return queue_.try_push(std::move(message));
}

inline void AsyncLogger::add_sink(std::shared_ptr<LogSink> sink) {
    std::lock_guard<std::mutex> lock(sinks_mutex_);
    sinks_.push_back(sink);
}

inline void AsyncLogger::remove_sink(std::shared_ptr<LogSink> sink) {
    std::lock_guard<std::mutex> lock(sinks_mutex_);
    auto it = std::find(sinks_.begin(), sinks_.end(), sink);
    if (it != sinks_.end()) {
        sinks_.erase(it);
    }
}

inline void AsyncLogger::clear_sinks() {
    std::lock_guard<std::mutex> lock(sinks_mutex_);
    sinks_.clear();
}

inline void AsyncLogger::flush() {
    std::lock_guard<std::mutex> lock(sinks_mutex_);
    for (auto& sink : sinks_) {
        sink->flush();
    }
    last_flush_time_ = std::chrono::steady_clock::now();
}

inline void AsyncLogger::set_level(LogLevel level) {
    std::lock_guard<std::mutex> lock(sinks_mutex_);
    for (auto& sink : sinks_) {
        sink->set_level(level);
    }
}

inline LogLevel AsyncLogger::get_level() const {
    std::lock_guard<std::mutex> lock(sinks_mutex_);
    if (sinks_.empty()) return LogLevel::INFO;
    return sinks_.front()->get_level();
}

inline bool AsyncLogger::should_log(LogLevel level) const {
    return level >= get_level();
}

inline size_t AsyncLogger::get_queue_size() const {
    return queue_.size();
}

inline size_t AsyncLogger::get_queue_capacity() const {
    return queue_.capacity();
}

inline size_t AsyncLogger::get_dropped_count() const {
    return dropped_count_.load();
}

inline double AsyncLogger::get_drop_rate() const {
    size_t total = get_queue_size() + get_dropped_count();
    return total > 0 ? static_cast<double>(get_dropped_count()) / total : 0.0;
}

inline void AsyncLogger::worker_thread_func() {
    std::vector<LogMessage> batch;
    batch.reserve(config_.batch_size);
    
    while (!stop_requested_) {
        batch.clear();
        
        // 收集批量消息
        LogMessage msg;
        size_t collected = 0;
        
        while (collected < config_.batch_size && !stop_requested_) {
            if (queue_.try_pop(msg)) {
                batch.push_back(std::move(msg));
                collected++;
            } else {
                // 队列为空，等待一段时间
                std::this_thread::sleep_for(std::chrono::milliseconds(1));
                break;
            }
        }
        
        if (!batch.empty()) {
            // 处理批量消息
            process_batch(batch);
        }
        
        // 检查是否需要自动刷新
        if (config_.auto_flush && should_auto_flush()) {
            flush();
        }
    }
    
    // 处理剩余的消息
    LogMessage remaining_msg;
    while (queue_.try_pop(remaining_msg)) {
        process_message(remaining_msg);
    }
}

inline void AsyncLogger::process_batch(const std::vector<LogMessage>& batch) {
    std::lock_guard<std::mutex> lock(sinks_mutex_);
    
    for (const auto& msg : batch) {
        for (auto& sink : sinks_) {
            if (sink->should_log(msg.level())) {
                sink->log(msg);
            }
        }
    }
}

inline void AsyncLogger::process_message(const LogMessage& msg) {
    std::lock_guard<std::mutex> lock(sinks_mutex_);
    
    for (auto& sink : sinks_) {
        if (sink->should_log(msg.level())) {
            sink->log(msg);
        }
    }
}

inline bool AsyncLogger::should_auto_flush() {
    auto now = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - last_flush_time_).count();
    return elapsed >= config_.flush_interval_ms;
}

} // namespace sugarlog 