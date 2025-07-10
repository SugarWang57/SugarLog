/**
 * @file log_sink.hpp
 * @brief 日志输出目标基类
 */

#pragma once

#include "log_message.hpp"
#include "log_level.hpp"
#include "log_formatter.hpp"
#include <memory>
#include <vector>
#include <mutex>
#include <functional>

namespace sugarlog {

/**
 * @brief 日志输出目标基类
 * 定义了所有日志输出目标的基本接口
 */
class LogSink {
public:
    /**
     * @brief 虚析构函数
     */
    virtual ~LogSink() = default;
    
    /**
     * @brief 输出日志消息
     * @param message 日志消息
     */
    virtual void log(const LogMessage& message) = 0;
    
    /**
     * @brief 设置日志级别
     * @param level 日志级别
     */
    virtual void set_level(LogLevel level) { level_ = level; }
    
    /**
     * @brief 获取日志级别
     * @return 日志级别
     */
    virtual LogLevel get_level() const { return level_; }
    
    /**
     * @brief 检查是否应该输出指定级别的日志
     * @param level 日志级别
     * @return 是否应该输出
     */
    virtual bool should_log(LogLevel level) const {
        return level >= level_;
    }
    
    /**
     * @brief 刷新输出缓冲区
     */
    virtual void flush() = 0;
    
    /**
     * @brief 设置格式化器
     * @param formatter 格式化器
     */
    virtual void set_formatter(std::shared_ptr<class LogFormatter> formatter) {
        formatter_ = formatter;
    }
    
    /**
     * @brief 获取格式化器
     * @return 格式化器
     */
    virtual std::shared_ptr<class LogFormatter> get_formatter() const {
        return formatter_;
    }
    
    /**
     * @brief 启用/禁用颜色输出
     * @param enabled 是否启用
     */
    virtual void set_colored(bool enabled) { colored_ = enabled; }
    
    /**
     * @brief 是否启用颜色输出
     * @return 是否启用
     */
    virtual bool is_colored() const { return colored_; }
    
    /**
     * @brief 设置是否启用异步输出
     * @param enabled 是否启用
     */
    virtual void set_async(bool enabled) { async_ = enabled; }
    
    /**
     * @brief 是否启用异步输出
     * @return 是否启用
     */
    virtual bool is_async() const { return async_; }
    
    /**
     * @brief 设置批量写入大小
     * @param batch_size 批量大小
     */
    virtual void set_batch_size(size_t batch_size) { batch_size_ = batch_size; }
    
    /**
     * @brief 获取批量写入大小
     * @return 批量大小
     */
    virtual size_t get_batch_size() const { return batch_size_; }
    
    /**
     * @brief 设置刷新间隔
     * @param interval_ms 刷新间隔（毫秒）
     */
    virtual void set_flush_interval(uint32_t interval_ms) { flush_interval_ms_ = interval_ms; }
    
    /**
     * @brief 获取刷新间隔
     * @return 刷新间隔（毫秒）
     */
    virtual uint32_t get_flush_interval() const { return flush_interval_ms_; }
    
    /**
     * @brief 启用/禁用敏感信息过滤
     * @param enabled 是否启用
     */
    virtual void set_sensitive_filter(bool enabled) { sensitive_filter_ = enabled; }
    
    /**
     * @brief 是否启用敏感信息过滤
     * @return 是否启用
     */
    virtual bool is_sensitive_filter_enabled() const { return sensitive_filter_; }
    
    /**
     * @brief 添加敏感关键词
     * @param keyword 敏感关键词
     */
    virtual void add_sensitive_keyword(const std::string& keyword);
    
    /**
     * @brief 移除敏感关键词
     * @param keyword 敏感关键词
     */
    virtual void remove_sensitive_keyword(const std::string& keyword);
    
    /**
     * @brief 清空敏感关键词列表
     */
    virtual void clear_sensitive_keywords();
    
    /**
     * @brief 获取敏感关键词列表
     * @return 敏感关键词列表
     */
    virtual const std::vector<std::string>& get_sensitive_keywords() const { return sensitive_keywords_; }
    
protected:
    LogLevel level_ = LogLevel::INFO;                    ///< 日志级别
    std::shared_ptr<class LogFormatter> formatter_;      ///< 格式化器
    bool colored_ = false;                               ///< 是否启用颜色
    bool async_ = false;                                 ///< 是否启用异步
    size_t batch_size_ = 100;                            ///< 批量写入大小
    uint32_t flush_interval_ms_ = 1000;                  ///< 刷新间隔
    bool sensitive_filter_ = false;                      ///< 是否启用敏感信息过滤
    std::vector<std::string> sensitive_keywords_;        ///< 敏感关键词列表
    mutable std::mutex mutex_;                           ///< 互斥锁
    
    /**
     * @brief 过滤敏感信息
     * @param message 原始消息
     * @return 过滤后的消息
     */
    std::string filter_sensitive_info(const std::string& message) const;
    
    /**
     * @brief 格式化日志消息
     * @param message 日志消息
     * @return 格式化后的字符串
     */
    std::string format_message(const LogMessage& message) const;
};

// LogSink 虚函数实现
inline void LogSink::add_sensitive_keyword(const std::string& keyword) {
    std::lock_guard<std::mutex> lock(mutex_);
    sensitive_keywords_.push_back(keyword);
}

inline void LogSink::remove_sensitive_keyword(const std::string& keyword) {
    std::lock_guard<std::mutex> lock(mutex_);
    auto it = std::find(sensitive_keywords_.begin(), sensitive_keywords_.end(), keyword);
    if (it != sensitive_keywords_.end()) {
        sensitive_keywords_.erase(it);
    }
}

inline void LogSink::clear_sensitive_keywords() {
    std::lock_guard<std::mutex> lock(mutex_);
    sensitive_keywords_.clear();
}

inline std::string LogSink::filter_sensitive_info(const std::string& message) const {
    if (!sensitive_filter_) return message;
    
    std::string filtered = message;
    for (const auto& keyword : sensitive_keywords_) {
        size_t pos = 0;
        while ((pos = filtered.find(keyword, pos)) != std::string::npos) {
            filtered.replace(pos, keyword.length(), "***");
            pos += 3;
        }
    }
    return filtered;
}

inline std::string LogSink::format_message(const LogMessage& message) const {
    if (formatter_) {
        return formatter_->format(message);
    }
    return message.message();
}

/**
 * @brief 复合日志输出目标
 * 可以包含多个子输出目标
 */
class CompositeLogSink : public LogSink {
public:
    /**
     * @brief 添加子输出目标
     * @param sink 子输出目标
     */
    void add_sink(std::shared_ptr<LogSink> sink);
    
    /**
     * @brief 移除子输出目标
     * @param sink 子输出目标
     */
    void remove_sink(std::shared_ptr<LogSink> sink);
    
    /**
     * @brief 清空所有子输出目标
     */
    void clear_sinks();
    
    /**
     * @brief 获取子输出目标列表
     * @return 子输出目标列表
     */
    const std::vector<std::shared_ptr<LogSink>>& get_sinks() const { return sinks_; }
    
    // 实现LogSink接口
    void log(const LogMessage& message) override;
    void flush() override;
    void set_level(LogLevel level) override;
    LogLevel get_level() const override;
    bool should_log(LogLevel level) const override;
    
private:
    std::vector<std::shared_ptr<LogSink>> sinks_;        ///< 子输出目标列表
    mutable std::mutex sinks_mutex_;                     ///< 子输出目标列表互斥锁
};

/**
 * @brief 过滤日志输出目标
 * 根据条件过滤日志消息
 */
class FilterLogSink : public LogSink {
public:
    using FilterFunction = std::function<bool(const LogMessage&)>;
    
    /**
     * @brief 构造函数
     * @param sink 底层输出目标
     * @param filter 过滤函数
     */
    FilterLogSink(std::shared_ptr<LogSink> sink, FilterFunction filter);
    
    /**
     * @brief 设置过滤函数
     * @param filter 过滤函数
     */
    void set_filter(FilterFunction filter);
    
    /**
     * @brief 获取过滤函数
     * @return 过滤函数
     */
    const FilterFunction& get_filter() const { return filter_; }
    
    // 实现LogSink接口
    void log(const LogMessage& message) override;
    void flush() override;
    void set_level(LogLevel level) override;
    LogLevel get_level() const override;
    bool should_log(LogLevel level) const override;
    
private:
    std::shared_ptr<LogSink> sink_;                      ///< 底层输出目标
    FilterFunction filter_;                              ///< 过滤函数
};

// CompositeLogSink 实现
inline void CompositeLogSink::add_sink(std::shared_ptr<LogSink> sink) {
    std::lock_guard<std::mutex> lock(sinks_mutex_);
    sinks_.push_back(sink);
}

inline void CompositeLogSink::remove_sink(std::shared_ptr<LogSink> sink) {
    std::lock_guard<std::mutex> lock(sinks_mutex_);
    auto it = std::find(sinks_.begin(), sinks_.end(), sink);
    if (it != sinks_.end()) {
        sinks_.erase(it);
    }
}

inline void CompositeLogSink::clear_sinks() {
    std::lock_guard<std::mutex> lock(sinks_mutex_);
    sinks_.clear();
}

inline void CompositeLogSink::log(const LogMessage& message) {
    std::lock_guard<std::mutex> lock(sinks_mutex_);
    for (auto& sink : sinks_) {
        if (sink && sink->should_log(message.level())) {
            sink->log(message);
        }
    }
}

inline void CompositeLogSink::flush() {
    std::lock_guard<std::mutex> lock(sinks_mutex_);
    for (auto& sink : sinks_) {
        if (sink) {
            sink->flush();
        }
    }
}

inline void CompositeLogSink::set_level(LogLevel level) {
    LogSink::set_level(level);
    std::lock_guard<std::mutex> lock(sinks_mutex_);
    for (auto& sink : sinks_) {
        if (sink) {
            sink->set_level(level);
        }
    }
}

inline LogLevel CompositeLogSink::get_level() const {
    return LogSink::get_level();
}

inline bool CompositeLogSink::should_log(LogLevel level) const {
    return LogSink::should_log(level);
}

// FilterLogSink 实现
inline FilterLogSink::FilterLogSink(std::shared_ptr<LogSink> sink, FilterFunction filter)
    : sink_(sink), filter_(filter) {}

inline void FilterLogSink::set_filter(FilterFunction filter) {
    filter_ = filter;
}

inline void FilterLogSink::log(const LogMessage& message) {
    if (filter_ && filter_(message)) {
        sink_->log(message);
    }
}

inline void FilterLogSink::flush() {
    sink_->flush();
}

inline void FilterLogSink::set_level(LogLevel level) {
    LogSink::set_level(level);
    sink_->set_level(level);
}

inline LogLevel FilterLogSink::get_level() const {
    return sink_->get_level();
}

inline bool FilterLogSink::should_log(LogLevel level) const {
    return sink_->should_log(level);
}

} // namespace sugarlog 