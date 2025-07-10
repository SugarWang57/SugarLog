/**
 * @file log_formatter.hpp
 * @brief 日志格式化器基类
 */

#pragma once

#include "log_message.hpp"
#include "log_level.hpp"
#include <string>
#include <memory>
#include <regex>
#include <unordered_map>
#include <mutex>
#include <functional>

namespace sugarlog {

/**
 * @brief 日志格式化器基类
 * 定义了日志消息格式化的基本接口
 */
class LogFormatter {
public:
    /**
     * @brief 虚析构函数
     */
    virtual ~LogFormatter() = default;
    
    /**
     * @brief 格式化日志消息
     * @param message 日志消息
     * @return 格式化后的字符串
     */
    virtual std::string format(const LogMessage& message) = 0;
    
    /**
     * @brief 设置格式字符串
     * @param pattern 格式字符串
     */
    virtual void set_pattern(const std::string& pattern) { pattern_ = pattern; }
    
    /**
     * @brief 获取格式字符串
     * @return 格式字符串
     */
    virtual const std::string& get_pattern() const { return pattern_; }
    
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
     * @brief 设置时间格式
     * @param format 时间格式
     */
    virtual void set_time_format(const std::string& format) { time_format_ = format; }
    
    /**
     * @brief 获取时间格式
     * @return 时间格式
     */
    virtual const std::string& get_time_format() const { return time_format_; }
    
    /**
     * @brief 设置是否显示毫秒
     * @param enabled 是否显示
     */
    virtual void set_show_milliseconds(bool enabled) { show_milliseconds_ = enabled; }
    
    /**
     * @brief 是否显示毫秒
     * @return 是否显示
     */
    virtual bool is_show_milliseconds() const { return show_milliseconds_; }
    
    /**
     * @brief 设置是否显示文件名
     * @param enabled 是否显示
     */
    virtual void set_show_filename(bool enabled) { show_filename_ = enabled; }
    
    /**
     * @brief 是否显示文件名
     * @return 是否显示
     */
    virtual bool is_show_filename() const { return show_filename_; }
    
    /**
     * @brief 设置是否显示函数名
     * @param enabled 是否显示
     */
    virtual void set_show_function(bool enabled) { show_function_ = enabled; }
    
    /**
     * @brief 是否显示函数名
     * @return 是否显示
     */
    virtual bool is_show_function() const { return show_function_; }
    
    /**
     * @brief 设置是否显示线程ID
     * @param enabled 是否显示
     */
    virtual void set_show_thread_id(bool enabled) { show_thread_id_ = enabled; }
    
    /**
     * @brief 是否显示线程ID
     * @return 是否显示
     */
    virtual bool is_show_thread_id() const { return show_thread_id_; }
    
protected:
    std::string pattern_ = "[%Y-%m-%d %H:%M:%S] [%l] [%t] %v";  ///< 格式字符串
    bool colored_ = false;                                       ///< 是否启用颜色
    std::string time_format_ = "%Y-%m-%d %H:%M:%S";             ///< 时间格式
    bool show_milliseconds_ = false;                             ///< 是否显示毫秒
    bool show_filename_ = true;                                  ///< 是否显示文件名
    bool show_function_ = true;                                  ///< 是否显示函数名
    bool show_thread_id_ = true;                                 ///< 是否显示线程ID
    
    /**
     * @brief 替换格式字符串中的占位符
     * @param pattern 格式字符串
     * @param message 日志消息
     * @return 替换后的字符串
     */
    std::string replace_pattern(const std::string& pattern, const LogMessage& message) const;
    
    /**
     * @brief 获取时间字符串
     * @param message 日志消息
     * @return 时间字符串
     */
    std::string get_time_string(const LogMessage& message) const;
    
    /**
     * @brief 获取级别字符串
     * @param message 日志消息
     * @return 级别字符串
     */
    std::string get_level_string(const LogMessage& message) const;
    
    /**
     * @brief 获取线程ID字符串
     * @param message 日志消息
     * @return 线程ID字符串
     */
    std::string get_thread_id_string(const LogMessage& message) const;
    
    /**
     * @brief 获取位置信息字符串
     * @param message 日志消息
     * @return 位置信息字符串
     */
    std::string get_location_string(const LogMessage& message) const;
};

/**
 * @brief 简单格式化器
 * 提供基本的日志格式化功能
 */
class SimpleFormatter : public LogFormatter {
public:
    /**
     * @brief 构造函数
     * @param pattern 格式字符串
     */
    explicit SimpleFormatter(const std::string& pattern = "[%Y-%m-%d %H:%M:%S] [%l] [%t] %v");
    
    /**
     * @brief 格式化日志消息
     * @param message 日志消息
     * @return 格式化后的字符串
     */
    std::string format(const LogMessage& message) override;
};

/**
 * @brief JSON格式化器
 * 将日志消息格式化为JSON格式
 */
class JsonFormatter : public LogFormatter {
public:
    /**
     * @brief 构造函数
     * @param pretty_print 是否美化输出
     */
    explicit JsonFormatter(bool pretty_print = false);
    
    /**
     * @brief 格式化日志消息
     * @param message 日志消息
     * @return 格式化后的JSON字符串
     */
    std::string format(const LogMessage& message) override;
    
    /**
     * @brief 设置是否美化输出
     * @param enabled 是否启用
     */
    void set_pretty_print(bool enabled) { pretty_print_ = enabled; }
    
    /**
     * @brief 是否美化输出
     * @return 是否启用
     */
    bool is_pretty_print() const { return pretty_print_; }
    
private:
    bool pretty_print_ = false;                                 ///< 是否美化输出
    
    /**
     * @brief 转义JSON字符串
     * @param str 原始字符串
     * @return 转义后的字符串
     */
    std::string escape_json(const std::string& str) const;
};

/**
 * @brief XML格式化器
 * 将日志消息格式化为XML格式
 */
class XmlFormatter : public LogFormatter {
public:
    /**
     * @brief 构造函数
     * @param pretty_print 是否美化输出
     */
    explicit XmlFormatter(bool pretty_print = false);
    
    /**
     * @brief 格式化日志消息
     * @param message 日志消息
     * @return 格式化后的XML字符串
     */
    std::string format(const LogMessage& message) override;
    
    /**
     * @brief 设置是否美化输出
     * @param enabled 是否启用
     */
    void set_pretty_print(bool enabled) { pretty_print_ = enabled; }
    
    /**
     * @brief 是否美化输出
     * @return 是否启用
     */
    bool is_pretty_print() const { return pretty_print_; }
    
private:
    bool pretty_print_ = false;                                 ///< 是否美化输出
    
    /**
     * @brief 转义XML字符串
     * @param str 原始字符串
     * @return 转义后的字符串
     */
    std::string escape_xml(const std::string& str) const;
};

/**
 * @brief 自定义格式化器
 * 支持自定义格式字符串的格式化器
 */
class CustomFormatter : public LogFormatter {
public:
    /**
     * @brief 构造函数
     * @param pattern 格式字符串
     */
    explicit CustomFormatter(const std::string& pattern);
    
    /**
     * @brief 格式化日志消息
     * @param message 日志消息
     * @return 格式化后的字符串
     */
    std::string format(const LogMessage& message) override;
    
    /**
     * @brief 添加自定义占位符
     * @param placeholder 占位符名称
     * @param handler 处理函数
     */
    void add_placeholder(const std::string& placeholder, 
                        std::function<std::string(const LogMessage&)> handler);
    
    /**
     * @brief 移除自定义占位符
     * @param placeholder 占位符名称
     */
    void remove_placeholder(const std::string& placeholder);
    
private:
    std::unordered_map<std::string, std::function<std::string(const LogMessage&)>> custom_placeholders_;
    mutable std::mutex placeholders_mutex_;
};

} // namespace sugarlog 