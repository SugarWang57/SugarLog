/**
 * @file logger.hpp
 * @brief 日志记录器
 */

#pragma once

#include "log_level.hpp"
#include "log_message.hpp"
#include "log_manager.hpp"
#include <string>
#include <memory>
#include <sstream> // Required for std::ostringstream
#include <algorithm> // Required for std::find
#include <vector> // Required for std::vector
#include <unordered_map> // Required for std::unordered_map

namespace sugarlog {

/**
 * @brief 日志器类
 * 提供便捷的日志记录接口
 */
class Logger {
public:
    /**
     * @brief 获取单例实例
     * @return 日志器实例
     */
    static Logger& get();
    
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
     * @brief 强制刷新所有日志
     */
    void flush();
    
    /**
     * @brief 获取日志管理器
     * @return 日志管理器引用
     */
    LogManager& get_manager() { return LogManager::get(); }
    
    /**
     * @brief 获取日志管理器（常量版本）
     * @return 日志管理器常量引用
     */
    const LogManager& get_manager() const { return LogManager::get(); }
    
    /**
     * @brief 设置日志器名称
     * @param name 日志器名称
     */
    void set_name(const std::string& name);
    
    /**
     * @brief 获取日志器名称
     * @return 日志器名称
     */
    const std::string& get_name() const { return name_; }
    
    /**
     * @brief 设置是否启用
     * @param enabled 是否启用
     */
    void set_enabled(bool enabled) { enabled_ = enabled; }
    
    /**
     * @brief 是否启用
     * @return 是否启用
     */
    bool is_enabled() const { return enabled_; }
    
    /**
     * @brief 设置是否启用性能监控
     * @param enabled 是否启用
     */
    void set_performance_monitoring(bool enabled) { performance_monitoring_ = enabled; }
    
    /**
     * @brief 是否启用性能监控
     * @return 是否启用
     */
    bool is_performance_monitoring() const { return performance_monitoring_; }
    
    /**
     * @brief 设置是否启用敏感信息过滤
     * @param enabled 是否启用
     */
    void set_sensitive_filter(bool enabled) { sensitive_filter_ = enabled; }
    
    /**
     * @brief 是否启用敏感信息过滤
     * @return 是否启用
     */
    bool is_sensitive_filter() const { return sensitive_filter_; }
    
    /**
     * @brief 添加敏感关键词
     * @param keyword 敏感关键词
     */
    void add_sensitive_keyword(const std::string& keyword);
    
    /**
     * @brief 移除敏感关键词
     * @param keyword 敏感关键词
     */
    void remove_sensitive_keyword(const std::string& keyword);
    
    /**
     * @brief 清空敏感关键词列表
     */
    void clear_sensitive_keywords();
    
    /**
     * @brief 获取敏感关键词列表
     * @return 敏感关键词列表
     */
    std::vector<std::string> get_sensitive_keywords() const;
    
    /**
     * @brief 设置是否启用上下文信息
     * @param enabled 是否启用
     */
    void set_context_enabled(bool enabled) { context_enabled_ = enabled; }
    
    /**
     * @brief 是否启用上下文信息
     * @return 是否启用
     */
    bool is_context_enabled() const { return context_enabled_; }
    
    /**
     * @brief 添加上下文信息
     * @param key 键
     * @param value 值
     */
    void add_context(const std::string& key, const std::string& value);
    
    /**
     * @brief 移除上下文信息
     * @param key 键
     */
    void remove_context(const std::string& key);
    
    /**
     * @brief 清空上下文信息
     */
    void clear_context();
    
    /**
     * @brief 获取上下文信息
     * @param key 键
     * @return 值
     */
    std::string get_context(const std::string& key) const;
    
    /**
     * @brief 获取所有上下文信息
     * @return 上下文信息映射
     */
    std::unordered_map<std::string, std::string> get_all_context() const;
    
    /**
     * @brief 设置是否启用结构化日志
     * @param enabled 是否启用
     */
    void set_structured_logging(bool enabled) { structured_logging_ = enabled; }
    
    /**
     * @brief 是否启用结构化日志
     * @return 是否启用
     */
    bool is_structured_logging() const { return structured_logging_; }
    
    /**
     * @brief 记录结构化日志
     * @param level 日志级别
     * @param event 事件名称
     * @param fields 字段映射
     * @param file 文件名
     * @param line 行号
     * @param function 函数名
     */
    void log_structured(LogLevel level,
                       const std::string& event,
                       const std::unordered_map<std::string, std::string>& fields,
                       const std::string& file = "",
                       uint32_t line = 0,
                       const std::string& function = "");
    
    /**
     * @brief 记录异常日志
     * @param level 日志级别
     * @param message 日志消息
     * @param exception 异常信息
     * @param file 文件名
     * @param line 行号
     * @param function 函数名
     */
    void log_exception(LogLevel level,
                      const std::string& message,
                      const std::exception& exception,
                      const std::string& file = "",
                      uint32_t line = 0,
                      const std::string& function = "");
    
    /**
     * @brief 记录性能日志
     * @param level 日志级别
     * @param operation 操作名称
     * @param duration_ns 持续时间（纳秒）
     * @param data_size_bytes 数据大小（字节）
     * @param file 文件名
     * @param line 行号
     * @param function 函数名
     */
    void log_performance(LogLevel level,
                        const std::string& operation,
                        uint64_t duration_ns,
                        size_t data_size_bytes = 0,
                        const std::string& file = "",
                        uint32_t line = 0,
                        const std::string& function = "");
    
    /**
     * @brief 记录审计日志
     * @param level 日志级别
     * @param user 用户
     * @param action 操作
     * @param resource 资源
     * @param result 结果
     * @param details 详细信息
     * @param file 文件名
     * @param line 行号
     * @param function 函数名
     */
    void log_audit(LogLevel level,
                   const std::string& user,
                   const std::string& action,
                   const std::string& resource,
                   const std::string& result,
                   const std::string& details = "",
                   const std::string& file = "",
                   uint32_t line = 0,
                   const std::string& function = "");
    
    /**
     * @brief 记录安全日志
     * @param level 日志级别
     * @param event 安全事件
     * @param source_ip 源IP
     * @param user_agent 用户代理
     * @param details 详细信息
     * @param file 文件名
     * @param line 行号
     * @param function 函数名
     */
    void log_security(LogLevel level,
                     const std::string& event,
                     const std::string& source_ip = "",
                     const std::string& user_agent = "",
                     const std::string& details = "",
                     const std::string& file = "",
                     uint32_t line = 0,
                     const std::string& function = "");
    
    /**
     * @brief 记录业务日志
     * @param level 日志级别
     * @param business_event 业务事件
     * @param business_id 业务ID
     * @param business_data 业务数据
     * @param file 文件名
     * @param line 行号
     * @param function 函数名
     */
    void log_business(LogLevel level,
                     const std::string& event,
                     const std::string& business_id,
                     const std::string& operation,
                     const std::string& details = "",
                     const std::string& file = "",
                     uint32_t line = 0,
                     const std::string& function = "");
    
    /**
     * @brief 记录调试日志
     * @param level 日志级别
     * @param message 日志消息
     * @param variables 变量映射
     * @param file 文件名
     * @param line 行号
     * @param function 函数名
     */
    void log_debug(LogLevel level,
                   const std::string& message,
                   const std::unordered_map<std::string, std::string>& variables,
                   const std::string& file = "",
                   uint32_t line = 0,
                   const std::string& function = "");
    
    /**
     * @brief 记录调用链日志
     * @param level 日志级别
     * @param trace_id 调用链ID
     * @param span_id 跨度ID
     * @param parent_span_id 父跨度ID
     * @param message 日志消息
     * @param file 文件名
     * @param line 行号
     * @param function 函数名
     */
    void log_trace(LogLevel level,
                   const std::string& trace_id,
                   const std::string& span_id,
                   const std::string& parent_span_id,
                   const std::string& message,
                   const std::string& file = "",
                   uint32_t line = 0,
                   const std::string& function = "");
    
    /**
     * @brief 记录指标日志
     * @param level 日志级别
     * @param metric_name 指标名称
     * @param metric_value 指标值
     * @param metric_unit 指标单位
     * @param tags 标签
     * @param file 文件名
     * @param line 行号
     * @param function 函数名
     */
    void log_metric(LogLevel level,
                    const std::string& metric_name,
                    double metric_value,
                    const std::string& metric_unit = "",
                    const std::unordered_map<std::string, std::string>& tags = {},
                    const std::string& file = "",
                    uint32_t line = 0,
                    const std::string& function = "");
    
private:
    Logger() = default;
    ~Logger() = default;
    
    std::string name_;                            ///< 日志器名称
    bool enabled_;                                ///< 是否启用
    bool performance_monitoring_;                 ///< 是否启用性能监控
    bool sensitive_filter_;                       ///< 是否启用敏感信息过滤
    bool context_enabled_;                        ///< 是否启用上下文信息
    bool structured_logging_;                     ///< 是否启用结构化日志
    std::unordered_map<std::string, std::string> context_; ///< 上下文信息
    std::vector<std::string> sensitive_keywords_; ///< 敏感关键词列表
    
    /**
     * @brief 过滤敏感信息
     * @param message 原始消息
     * @return 过滤后的消息
     */
    std::string filter_sensitive_info(const std::string& message) const;
    
    /**
     * @brief 添加上下文到消息
     * @param message 原始消息
     * @return 添加上下文后的消息
     */
    std::string add_context_to_message(const std::string& message) const;
    
    /**
     * @brief 格式化结构化日志
     * @param event 事件名称
     * @param fields 字段映射
     * @return 格式化后的消息
     */
    std::string format_structured_log(const std::string& event,
                                     const std::unordered_map<std::string, std::string>& fields) const;
    
    /**
     * @brief 格式化异常日志
     * @param message 日志消息
     * @param exception 异常信息
     * @return 格式化后的消息
     */
    std::string format_exception_log(const std::string& message,
                                    const std::exception& exception) const;
    
    /**
     * @brief 格式化性能日志
     * @param operation 操作名称
     * @param duration_ns 持续时间（纳秒）
     * @param data_size_bytes 数据大小（字节）
     * @return 格式化后的消息
     */
    std::string format_performance_log(const std::string& operation,
                                      uint64_t duration_ns,
                                      size_t data_size_bytes) const;
    
    /**
     * @brief 格式化审计日志
     * @param user 用户
     * @param action 操作
     * @param resource 资源
     * @param result 结果
     * @param details 详细信息
     * @return 格式化后的消息
     */
    std::string format_audit_log(const std::string& user,
                                 const std::string& action,
                                 const std::string& resource,
                                 const std::string& result,
                                 const std::string& details) const;
    
    /**
     * @brief 格式化安全日志
     * @param event 安全事件
     * @param source_ip 源IP
     * @param user_agent 用户代理
     * @param details 详细信息
     * @return 格式化后的消息
     */
    std::string format_security_log(const std::string& event,
                                    const std::string& source_ip,
                                    const std::string& user_agent,
                                    const std::string& details) const;
    
    /**
     * @brief 格式化业务日志
     * @param business_event 业务事件
     * @param business_id 业务ID
     * @param business_data 业务数据
     * @return 格式化后的消息
     */
    std::string format_business_log(const std::string& business_event,
                                    const std::string& business_id,
                                    const std::string& business_data) const;
    
    /**
     * @brief 格式化调试日志
     * @param message 日志消息
     * @param variables 变量映射
     * @return 格式化后的消息
     */
    std::string format_debug_log(const std::string& message,
                                 const std::unordered_map<std::string, std::string>& variables) const;
    
    /**
     * @brief 格式化调用链日志
     * @param trace_id 调用链ID
     * @param span_id 跨度ID
     * @param parent_span_id 父跨度ID
     * @param message 日志消息
     * @return 格式化后的消息
     */
    std::string format_trace_log(const std::string& trace_id,
                                 const std::string& span_id,
                                 const std::string& parent_span_id,
                                 const std::string& message) const;
    
    /**
     * @brief 格式化指标日志
     * @param metric_name 指标名称
     * @param metric_value 指标值
     * @param metric_unit 指标单位
     * @param tags 标签
     * @return 格式化后的消息
     */
    std::string format_metric_log(const std::string& metric_name,
                                  double metric_value,
                                  const std::string& metric_unit,
                                  const std::unordered_map<std::string, std::string>& tags) const;
    
    // 禁用拷贝和赋值
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;
};

// 单例实现
inline Logger& Logger::get() {
    static Logger instance;
    return instance;
}

// 成员函数实现
inline void Logger::log(LogLevel level, 
                       const std::string& message,
                       const std::string& file,
                       uint32_t line,
                       const std::string& function) {
    if (!enabled_ || !should_log(level)) {
        return;
    }
    
    std::string filtered_message = sensitive_filter_ ? filter_sensitive_info(message) : message;
    LogManager::get().log(level, filtered_message, file, line, function);
}

inline void Logger::trace(const std::string& message,
                         const std::string& file,
                         uint32_t line,
                         const std::string& function) {
    log(LogLevel::TRACE, message, file, line, function);
}

inline void Logger::debug(const std::string& message,
                         const std::string& file,
                         uint32_t line,
                         const std::string& function) {
    log(LogLevel::DEBUG, message, file, line, function);
}

inline void Logger::info(const std::string& message,
                        const std::string& file,
                        uint32_t line,
                        const std::string& function) {
    log(LogLevel::INFO, message, file, line, function);
}

inline void Logger::warn(const std::string& message,
                        const std::string& file,
                        uint32_t line,
                        const std::string& function) {
    log(LogLevel::WARN, message, file, line, function);
}

inline void Logger::error(const std::string& message,
                         const std::string& file,
                         uint32_t line,
                         const std::string& function) {
    log(LogLevel::ERROR, message, file, line, function);
}

inline void Logger::fatal(const std::string& message,
                         const std::string& file,
                         uint32_t line,
                         const std::string& function) {
    log(LogLevel::FATAL, message, file, line, function);
}

inline void Logger::set_level(LogLevel level) {
    LogManager::get().set_level(level);
}

inline LogLevel Logger::get_level() const {
    return LogManager::get().get_level();
}

inline bool Logger::should_log(LogLevel level) const {
    return LogManager::get().should_log(level);
}

inline void Logger::flush() {
    LogManager::get().flush();
}

inline void Logger::set_name(const std::string& name) {
    name_ = name;
}

inline void Logger::add_sensitive_keyword(const std::string& keyword) {
    sensitive_keywords_.push_back(keyword);
}

inline void Logger::remove_sensitive_keyword(const std::string& keyword) {
    auto it = std::find(sensitive_keywords_.begin(), sensitive_keywords_.end(), keyword);
    if (it != sensitive_keywords_.end()) {
        sensitive_keywords_.erase(it);
    }
}

inline void Logger::clear_sensitive_keywords() {
    sensitive_keywords_.clear();
}

inline std::vector<std::string> Logger::get_sensitive_keywords() const {
    return sensitive_keywords_;
}

inline void Logger::add_context(const std::string& key, const std::string& value) {
    context_[key] = value;
}

inline void Logger::remove_context(const std::string& key) {
    context_.erase(key);
}

inline void Logger::clear_context() {
    context_.clear();
}

inline std::string Logger::get_context(const std::string& key) const {
    auto it = context_.find(key);
    return it != context_.end() ? it->second : "";
}

inline std::unordered_map<std::string, std::string> Logger::get_all_context() const {
    return context_;
}

inline void Logger::log_structured(LogLevel level,
                                 const std::string& event,
                                 const std::unordered_map<std::string, std::string>& fields,
                                 const std::string& file,
                                 uint32_t line,
                                 const std::string& function) {
    if (!enabled_ || !should_log(level)) {
        return;
    }
    
    std::ostringstream oss;
    oss << "STRUCTURED_LOG: " << event;
    for (const auto& field : fields) {
        oss << " " << field.first << "=" << field.second;
    }
    
    log(level, oss.str(), file, line, function);
}

inline void Logger::log_exception(LogLevel level,
                                const std::string& message,
                                const std::exception& exception,
                                const std::string& file,
                                uint32_t line,
                                const std::string& function) {
    if (!enabled_ || !should_log(level)) {
        return;
    }
    
    std::string full_message = message + " Exception: " + exception.what();
    log(level, full_message, file, line, function);
}

inline void Logger::log_performance(LogLevel level,
                                  const std::string& operation,
                                  uint64_t duration_ns,
                                  size_t data_size_bytes,
                                  const std::string& file,
                                  uint32_t line,
                                  const std::string& function) {
    if (!enabled_ || !should_log(level)) {
        return;
    }
    
    std::ostringstream oss;
    oss << "PERFORMANCE: " << operation << " duration=" << duration_ns << "ns";
    if (data_size_bytes > 0) {
        oss << " data_size=" << data_size_bytes << "bytes";
    }
    
    log(level, oss.str(), file, line, function);
}

inline void Logger::log_audit(LogLevel level,
                             const std::string& user,
                             const std::string& action,
                             const std::string& resource,
                             const std::string& result,
                             const std::string& details,
                             const std::string& file,
                             uint32_t line,
                             const std::string& function) {
    if (!enabled_ || !should_log(level)) {
        return;
    }
    
    std::ostringstream oss;
    oss << "AUDIT: user=" << user << " action=" << action << " resource=" << resource << " result=" << result;
    if (!details.empty()) {
        oss << " details=" << details;
    }
    
    log(level, oss.str(), file, line, function);
}

inline void Logger::log_security(LogLevel level,
                                const std::string& event,
                                const std::string& source_ip,
                                const std::string& user_agent,
                                const std::string& details,
                                const std::string& file,
                                uint32_t line,
                                const std::string& function) {
    if (!enabled_ || !should_log(level)) {
        return;
    }
    
    std::ostringstream oss;
    oss << "SECURITY: event=" << event << " source_ip=" << source_ip << " user_agent=" << user_agent;
    if (!details.empty()) {
        oss << " details=" << details;
    }
    
    log(level, oss.str(), file, line, function);
}

inline void Logger::log_business(LogLevel level,
                                const std::string& event,
                                const std::string& business_id,
                                const std::string& operation,
                                const std::string& details,
                                const std::string& file,
                                uint32_t line,
                                const std::string& function) {
    if (!enabled_ || !should_log(level)) {
        return;
    }
    
    std::ostringstream oss;
    oss << "BUSINESS: event=" << event << " business_id=" << business_id << " operation=" << operation;
    if (!details.empty()) {
        oss << " details=" << details;
    }
    
    log(level, oss.str(), file, line, function);
}

inline void Logger::log_debug(LogLevel level,
                             const std::string& message,
                             const std::unordered_map<std::string, std::string>& debug_info,
                             const std::string& file,
                             uint32_t line,
                             const std::string& function) {
    if (!enabled_ || !should_log(level)) {
        return;
    }
    
    std::ostringstream oss;
    oss << "DEBUG: " << message;
    for (const auto& info : debug_info) {
        oss << " " << info.first << "=" << info.second;
    }
    
    log(level, oss.str(), file, line, function);
}

inline void Logger::log_trace(LogLevel level,
                             const std::string& trace_id,
                             const std::string& span_id,
                             const std::string& parent_span_id,
                             const std::string& message,
                             const std::string& file,
                             uint32_t line,
                             const std::string& function) {
    if (!enabled_ || !should_log(level)) {
        return;
    }
    
    std::ostringstream oss;
    oss << "TRACE: trace_id=" << trace_id << " span_id=" << span_id << " parent_span_id=" << parent_span_id << " " << message;
    
    log(level, oss.str(), file, line, function);
}

inline void Logger::log_metric(LogLevel level,
                              const std::string& metric_name,
                              double metric_value,
                              const std::string& metric_unit,
                              const std::unordered_map<std::string, std::string>& tags,
                              const std::string& file,
                              uint32_t line,
                              const std::string& function) {
    if (!enabled_ || !should_log(level)) {
        return;
    }
    
    std::ostringstream oss;
    oss << "METRIC: " << metric_name << "=" << metric_value;
    if (!metric_unit.empty()) {
        oss << metric_unit;
    }
    for (const auto& tag : tags) {
        oss << " " << tag.first << "=" << tag.second;
    }
    
    log(level, oss.str(), file, line, function);
}

inline std::string Logger::filter_sensitive_info(const std::string& message) const {
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

} // namespace sugarlog 