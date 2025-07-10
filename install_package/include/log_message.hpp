/**
 * @file log_message.hpp
 * @brief 日志消息类定义
 */

#pragma once

#include "log_level.hpp"
#include <string>
#include <chrono>
#include <memory>
#include <sstream>
#include <thread>

namespace sugarlog {

/**
 * @brief 日志消息类
 * 包含完整的日志信息：时间戳、级别、消息内容、线程ID等
 */
class LogMessage {
public:
    using Clock = std::chrono::system_clock;
    using TimePoint = Clock::time_point;
    
    /**
     * @brief 构造函数
     * @param level 日志级别
     * @param message 日志消息
     * @param file 文件名
     * @param line 行号
     * @param function 函数名
     */
    LogMessage(LogLevel level, 
               std::string message,
               std::string file = "",
               uint32_t line = 0,
               std::string function = "")
        : level_(level)
        , message_(std::move(message))
        , file_(std::move(file))
        , line_(line)
        , function_(std::move(function))
        , timestamp_(Clock::now())
        , thread_id_(std::this_thread::get_id())
    {}
    
    /**
     * @brief 移动构造函数
     */
    LogMessage(LogMessage&&) = default;
    
    /**
     * @brief 移动赋值操作符
     */
    LogMessage& operator=(LogMessage&&) = default;
    
    /**
     * @brief 禁用拷贝构造和赋值
     */
    LogMessage(const LogMessage&) = delete;
    LogMessage& operator=(const LogMessage&) = delete;
    
    /**
     * @brief 默认构造函数
     */
    LogMessage() : level_(LogLevel::INFO), line_(0), timestamp_(Clock::now()), thread_id_(std::this_thread::get_id()) {}
    
    /**
     * @brief 析构函数
     */
    ~LogMessage() = default;
    
    // Getters
    LogLevel level() const { return level_; }
    const std::string& message() const { return message_; }
    const std::string& file() const { return file_; }
    uint32_t line() const { return line_; }
    const std::string& function() const { return function_; }
    TimePoint timestamp() const { return timestamp_; }
    std::thread::id thread_id() const { return thread_id_; }
    
    /**
     * @brief 获取时间戳的字符串表示
     * @param format 时间格式，默认为ISO格式
     * @return 时间字符串
     */
    std::string timestamp_string(const std::string& format = "%Y-%m-%d %H:%M:%S") const;
    
    /**
     * @brief 获取线程ID的字符串表示
     * @return 线程ID字符串
     */
    std::string thread_id_string() const;
    
    /**
     * @brief 获取文件名（不包含路径）
     * @return 文件名
     */
    std::string filename() const;
    
    /**
     * @brief 获取完整的日志信息字符串
     * @return 格式化的日志字符串
     */
    std::string to_string() const;
    
    /**
     * @brief 获取JSON格式的日志信息
     * @return JSON字符串
     */
    std::string to_json() const;
    
    /**
     * @brief 获取XML格式的日志信息
     * @return XML字符串
     */
    std::string to_xml() const;
    
    /**
     * @brief 检查是否包含敏感信息
     * @return 是否包含敏感信息
     */
    bool contains_sensitive_info() const;
    
    /**
     * @brief 脱敏处理
     * @return 脱敏后的消息
     */
    std::string sanitize() const;
    
private:
    LogLevel level_;                    ///< 日志级别
    std::string message_;               ///< 日志消息内容
    std::string file_;                  ///< 文件名
    uint32_t line_;                     ///< 行号
    std::string function_;              ///< 函数名
    TimePoint timestamp_;               ///< 时间戳
    std::thread::id thread_id_;         ///< 线程ID
    
    /**
     * @brief 检测敏感关键词
     * @param text 待检测文本
     * @return 是否包含敏感信息
     */
    static bool contains_sensitive_keywords(const std::string& text);
    
    /**
     * @brief 替换敏感信息
     * @param text 原始文本
     * @return 替换后的文本
     */
    static std::string replace_sensitive_info(const std::string& text);
};

/**
 * @brief 日志消息构建器
 * 提供流式接口来构建日志消息
 */
class LogMessageBuilder {
public:
    /**
     * @brief 构造函数
     * @param level 日志级别
     * @param file 文件名
     * @param line 行号
     * @param function 函数名
     */
    LogMessageBuilder(LogLevel level, 
                     std::string file = "",
                     uint32_t line = 0,
                     std::string function = "")
        : level_(level)
        , file_(std::move(file))
        , line_(line)
        , function_(std::move(function))
    {}
    
    /**
     * @brief 添加消息内容
     * @param message 消息内容
     * @return 构建器引用
     */
    LogMessageBuilder& message(const std::string& message) {
        message_ = message;
        return *this;
    }
    
    /**
     * @brief 流式添加消息内容
     * @param value 要添加的值
     * @return 构建器引用
     */
    template<typename T>
    LogMessageBuilder& operator<<(const T& value) {
        stream_ << value;
        return *this;
    }
    
    /**
     * @brief 构建LogMessage对象
     * @return LogMessage对象
     */
    LogMessage build() {
        std::string final_message = message_;
        if (!stream_.str().empty()) {
            if (!final_message.empty()) {
                final_message += " ";
            }
            final_message += stream_.str();
        }
        return LogMessage(level_, std::move(final_message), 
                         std::move(file_), line_, std::move(function_));
    }
    
private:
    LogLevel level_;
    std::string message_;
    std::string file_;
    uint32_t line_;
    std::string function_;
    std::ostringstream stream_;
};

} // namespace sugarlog 