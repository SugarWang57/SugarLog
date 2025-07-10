/**
 * @file log_level.hpp
 * @brief 日志级别定义
 */

#pragma once

#include <string>
#include <ostream>
#include <algorithm>
#include <stdexcept>

namespace sugarlog {

/**
 * @brief 日志级别枚举
 */
enum class LogLevel : uint8_t {
    TRACE = 0,    ///< 跟踪级别 - 最详细的日志信息
    DEBUG = 1,    ///< 调试级别 - 调试信息
    INFO = 2,     ///< 信息级别 - 一般信息
    WARN = 3,     ///< 警告级别 - 警告信息
    ERROR = 4,    ///< 错误级别 - 错误信息
    FATAL = 5,    ///< 致命级别 - 致命错误
    OFF = 6       ///< 关闭级别 - 不输出任何日志
};

/**
 * @brief 日志级别转换为字符串
 * @param level 日志级别
 * @return 字符串表示
 */
inline std::string to_string(LogLevel level) {
    switch (level) {
        case LogLevel::TRACE: return "TRACE";
        case LogLevel::DEBUG: return "DEBUG";
        case LogLevel::INFO:  return "INFO";
        case LogLevel::WARN:  return "WARN";
        case LogLevel::ERROR: return "ERROR";
        case LogLevel::FATAL: return "FATAL";
        case LogLevel::OFF:   return "OFF";
        default:              return "UNKNOWN";
    }
}

/**
 * @brief 日志级别转换为短字符串
 * @param level 日志级别
 * @return 短字符串表示
 */
inline std::string to_short_string(LogLevel level) {
    switch (level) {
        case LogLevel::TRACE: return "T";
        case LogLevel::DEBUG: return "D";
        case LogLevel::INFO:  return "I";
        case LogLevel::WARN:  return "W";
        case LogLevel::ERROR: return "E";
        case LogLevel::FATAL: return "F";
        case LogLevel::OFF:   return "O";
        default:              return "?";
    }
}

/**
 * @brief 从字符串解析日志级别
 * @param str 字符串
 * @return 日志级别
 */
inline LogLevel from_string(const std::string& str) {
    std::string upper = str;
    std::transform(upper.begin(), upper.end(), upper.begin(), ::toupper);
    
    if (upper == "TRACE") return LogLevel::TRACE;
    if (upper == "DEBUG") return LogLevel::DEBUG;
    if (upper == "INFO")  return LogLevel::INFO;
    if (upper == "WARN")  return LogLevel::WARN;
    if (upper == "ERROR") return LogLevel::ERROR;
    if (upper == "FATAL") return LogLevel::FATAL;
    if (upper == "OFF")   return LogLevel::OFF;
    
    throw std::invalid_argument("Invalid log level: " + str);
}

/**
 * @brief 获取日志级别的颜色代码（用于控制台输出）
 * @param level 日志级别
 * @return ANSI颜色代码
 */
inline std::string get_color_code(LogLevel level) {
    switch (level) {
        case LogLevel::TRACE: return "\033[37m"; // 白色
        case LogLevel::DEBUG: return "\033[36m"; // 青色
        case LogLevel::INFO:  return "\033[32m"; // 绿色
        case LogLevel::WARN:  return "\033[33m"; // 黄色
        case LogLevel::ERROR: return "\033[31m"; // 红色
        case LogLevel::FATAL: return "\033[35m"; // 紫色
        default:              return "\033[0m";  // 重置
    }
}

/**
 * @brief 重置颜色代码
 */
inline const std::string& get_reset_color() {
    static const std::string reset = "\033[0m";
    return reset;
}

/**
 * @brief 比较两个日志级别
 * @param lhs 左操作数
 * @param rhs 右操作数
 * @return 是否lhs >= rhs
 */
inline bool operator>=(LogLevel lhs, LogLevel rhs) {
    return static_cast<uint8_t>(lhs) >= static_cast<uint8_t>(rhs);
}

/**
 * @brief 比较两个日志级别
 * @param lhs 左操作数
 * @param rhs 右操作数
 * @return 是否lhs > rhs
 */
inline bool operator>(LogLevel lhs, LogLevel rhs) {
    return static_cast<uint8_t>(lhs) > static_cast<uint8_t>(rhs);
}

/**
 * @brief 比较两个日志级别
 * @param lhs 左操作数
 * @param rhs 右操作数
 * @return 是否lhs <= rhs
 */
inline bool operator<=(LogLevel lhs, LogLevel rhs) {
    return static_cast<uint8_t>(lhs) <= static_cast<uint8_t>(rhs);
}

/**
 * @brief 比较两个日志级别
 * @param lhs 左操作数
 * @param rhs 右操作数
 * @return 是否lhs < rhs
 */
inline bool operator<(LogLevel lhs, LogLevel rhs) {
    return static_cast<uint8_t>(lhs) < static_cast<uint8_t>(rhs);
}

/**
 * @brief 输出流操作符
 * @param os 输出流
 * @param level 日志级别
 * @return 输出流
 */
inline std::ostream& operator<<(std::ostream& os, LogLevel level) {
    os << to_string(level);
    return os;
}

} // namespace sugarlog 