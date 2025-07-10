/**
 * @file console_sink.hpp
 * @brief 控制台输出目标
 */

#pragma once

#include "log_sink.hpp"
#include <iostream>
#include <mutex>

namespace sugarlog {

/**
 * @brief 控制台输出目标
 * 将日志输出到标准输出或标准错误
 */
class ConsoleSink : public LogSink {
public:
    /**
     * @brief 输出流类型
     */
    enum class StreamType {
        STDOUT,     ///< 标准输出
        STDERR      ///< 标准错误
    };
    
    /**
     * @brief 构造函数
     * @param stream_type 输出流类型
     * @param colored 是否启用颜色
     */
    explicit ConsoleSink(StreamType stream_type = StreamType::STDOUT, 
                        bool colored = true);
    
    /**
     * @brief 析构函数
     */
    ~ConsoleSink() override = default;
    
    /**
     * @brief 输出日志消息
     * @param message 日志消息
     */
    void log(const LogMessage& message) override;
    
    /**
     * @brief 刷新输出缓冲区
     */
    void flush() override;
    
    /**
     * @brief 设置输出流类型
     * @param stream_type 输出流类型
     */
    void set_stream_type(StreamType stream_type);
    
    /**
     * @brief 获取输出流类型
     * @return 输出流类型
     */
    StreamType get_stream_type() const { return stream_type_; }
    
    /**
     * @brief 设置是否启用颜色
     * @param enabled 是否启用
     */
    void set_colored(bool enabled) override;
    
    /**
     * @brief 设置是否启用自动刷新
     * @param enabled 是否启用
     */
    void set_auto_flush(bool enabled) { auto_flush_ = enabled; }
    
    /**
     * @brief 是否启用自动刷新
     * @return 是否启用
     */
    bool is_auto_flush() const { return auto_flush_; }
    
    /**
     * @brief 设置是否启用线程安全
     * @param enabled 是否启用
     */
    void set_thread_safe(bool enabled) { thread_safe_ = enabled; }
    
    /**
     * @brief 是否启用线程安全
     * @return 是否启用
     */
    bool is_thread_safe() const { return thread_safe_; }
    
    /**
     * @brief 设置输出缓冲区大小
     * @param size 缓冲区大小
     */
    void set_buffer_size(size_t size);
    
    /**
     * @brief 获取输出缓冲区大小
     * @return 缓冲区大小
     */
    size_t get_buffer_size() const { return buffer_size_; }
    
    /**
     * @brief 设置是否启用时间戳
     * @param enabled 是否启用
     */
    void set_show_timestamp(bool enabled) { show_timestamp_ = enabled; }
    
    /**
     * @brief 是否启用时间戳
     * @return 是否启用
     */
    bool is_show_timestamp() const { return show_timestamp_; }
    
    /**
     * @brief 设置是否启用级别
     * @param enabled 是否启用
     */
    void set_show_level(bool enabled) { show_level_ = enabled; }
    
    /**
     * @brief 是否启用级别
     * @return 是否启用
     */
    bool is_show_level() const { return show_level_; }
    
    /**
     * @brief 设置是否启用线程ID
     * @param enabled 是否启用
     */
    void set_show_thread_id(bool enabled) { show_thread_id_ = enabled; }
    
    /**
     * @brief 是否启用线程ID
     * @return 是否启用
     */
    bool is_show_thread_id() const { return show_thread_id_; }
    
    /**
     * @brief 设置是否启用位置信息
     * @param enabled 是否启用
     */
    void set_show_location(bool enabled) { show_location_ = enabled; }
    
    /**
     * @brief 是否启用位置信息
     * @return 是否启用
     */
    bool is_show_location() const { return show_location_; }
    
    /**
     * @brief 设置是否启用函数名
     * @param enabled 是否启用
     */
    void set_show_function(bool enabled) { show_function_ = enabled; }
    
    /**
     * @brief 是否启用函数名
     * @return 是否启用
     */
    bool is_show_function() const { return show_function_; }
    
    /**
     * @brief 设置是否启用文件名
     * @param enabled 是否启用
     */
    void set_show_filename(bool enabled) { show_filename_ = enabled; }
    
    /**
     * @brief 是否启用文件名
     * @return 是否启用
     */
    bool is_show_filename() const { return show_filename_; }
    
    /**
     * @brief 设置是否启用行号
     * @param enabled 是否启用
     */
    void set_show_line_number(bool enabled) { show_line_number_ = enabled; }
    
    /**
     * @brief 是否启用行号
     * @return 是否启用
     */
    bool is_show_line_number() const { return show_line_number_; }
    
    /**
     * @brief 设置是否启用毫秒
     * @param enabled 是否启用
     */
    void set_show_milliseconds(bool enabled) { show_milliseconds_ = enabled; }
    
    /**
     * @brief 是否启用毫秒
     * @return 是否启用
     */
    bool is_show_milliseconds() const { return show_milliseconds_; }
    
    /**
     * @brief 设置是否启用分隔符
     * @param enabled 是否启用
     */
    void set_show_separator(bool enabled) { show_separator_ = enabled; }
    
    /**
     * @brief 是否启用分隔符
     * @return 是否启用
     */
    bool is_show_separator() const { return show_separator_; }
    
    /**
     * @brief 设置分隔符
     * @param separator 分隔符
     */
    void set_separator(const std::string& separator) { separator_ = separator; }
    
    /**
     * @brief 获取分隔符
     * @return 分隔符
     */
    const std::string& get_separator() const { return separator_; }
    
    /**
     * @brief 设置是否启用换行
     * @param enabled 是否启用
     */
    void set_show_newline(bool enabled) { show_newline_ = enabled; }
    
    /**
     * @brief 是否启用换行
     * @return 是否启用
     */
    bool is_show_newline() const { return show_newline_; }
    
    /**
     * @brief 设置是否启用前缀
     * @param enabled 是否启用
     */
    void set_show_prefix(bool enabled) { show_prefix_ = enabled; }
    
    /**
     * @brief 是否启用前缀
     * @return 是否启用
     */
    bool is_show_prefix() const { return show_prefix_; }
    
    /**
     * @brief 设置前缀
     * @param prefix 前缀
     */
    void set_prefix(const std::string& prefix) { prefix_ = prefix; }
    
    /**
     * @brief 获取前缀
     * @return 前缀
     */
    const std::string& get_prefix() const { return prefix_; }
    
    /**
     * @brief 设置是否启用后缀
     * @param enabled 是否启用
     */
    void set_show_suffix(bool enabled) { show_suffix_ = enabled; }
    
    /**
     * @brief 是否启用后缀
     * @return 是否启用
     */
    bool is_show_suffix() const { return show_suffix_; }
    
    /**
     * @brief 设置后缀
     * @param suffix 后缀
     */
    void set_suffix(const std::string& suffix) { suffix_ = suffix; }
    
    /**
     * @brief 获取后缀
     * @return 后缀
     */
    const std::string& get_suffix() const { return suffix_; }
    
    /**
     * @brief 设置是否启用进度条
     * @param enabled 是否启用
     */
    void set_show_progress(bool enabled) { show_progress_ = enabled; }
    
    /**
     * @brief 是否启用进度条
     * @return 是否启用
     */
    bool is_show_progress() const { return show_progress_; }
    
    /**
     * @brief 设置进度条宽度
     * @param width 宽度
     */
    void set_progress_width(size_t width) { progress_width_ = width; }
    
    /**
     * @brief 获取进度条宽度
     * @return 宽度
     */
    size_t get_progress_width() const { return progress_width_; }
    
    /**
     * @brief 设置是否启用统计信息
     * @param enabled 是否启用
     */
    void set_show_stats(bool enabled) { show_stats_ = enabled; }
    
    /**
     * @brief 是否启用统计信息
     * @return 是否启用
     */
    bool is_show_stats() const { return show_stats_; }
    
    /**
     * @brief 设置统计信息间隔
     * @param interval 间隔（日志条数）
     */
    void set_stats_interval(size_t interval) { stats_interval_ = interval; }
    
    /**
     * @brief 获取统计信息间隔
     * @return 间隔
     */
    size_t get_stats_interval() const { return stats_interval_; }
    
protected:
    StreamType stream_type_;                      ///< 输出流类型
    std::ostream* stream_;                        ///< 输出流指针
    bool auto_flush_;                             ///< 是否自动刷新
    bool thread_safe_;                            ///< 是否线程安全
    size_t buffer_size_;                          ///< 缓冲区大小
    bool show_timestamp_;                         ///< 是否显示时间戳
    bool show_level_;                             ///< 是否显示级别
    bool show_thread_id_;                         ///< 是否显示线程ID
    bool show_location_;                          ///< 是否显示位置信息
    bool show_function_;                          ///< 是否显示函数名
    bool show_filename_;                          ///< 是否显示文件名
    bool show_line_number_;                       ///< 是否显示行号
    bool show_milliseconds_;                      ///< 是否显示毫秒
    bool show_separator_;                         ///< 是否显示分隔符
    std::string separator_;                       ///< 分隔符
    bool show_newline_;                           ///< 是否显示换行
    bool show_prefix_;                            ///< 是否显示前缀
    std::string prefix_;                          ///< 前缀
    bool show_suffix_;                            ///< 是否显示后缀
    std::string suffix_;                          ///< 后缀
    bool show_progress_;                          ///< 是否显示进度条
    size_t progress_width_;                       ///< 进度条宽度
    bool show_stats_;                             ///< 是否显示统计信息
    size_t stats_interval_;                       ///< 统计信息间隔
    std::atomic<uint64_t> log_count_;             ///< 日志计数
    std::chrono::steady_clock::time_point last_stats_time_; ///< 上次统计时间
    mutable std::mutex console_mutex_;            ///< 控制台互斥锁
    
    /**
     * @brief 获取输出流
     * @return 输出流引用
     */
    std::ostream& get_stream();
    
    /**
     * @brief 格式化日志消息
     * @param message 日志消息
     * @return 格式化后的字符串
     */
    std::string format_message(const LogMessage& message);
    
    /**
     * @brief 输出带颜色的文本
     * @param text 文本内容
     * @param color_code 颜色代码
     */
    void output_colored(const std::string& text, const std::string& color_code);
    
    /**
     * @brief 输出进度条
     * @param current 当前值
     * @param total 总值
     */
    void output_progress(size_t current, size_t total);
    
    /**
     * @brief 输出统计信息
     */
    void output_stats();
    
    /**
     * @brief 检查是否需要输出统计信息
     * @return 是否需要输出
     */
    bool should_output_stats();
    
    /**
     * @brief 更新统计信息
     */
    void update_stats();
};

// ConsoleSink 实现
inline ConsoleSink::ConsoleSink(StreamType stream_type, bool colored)
    : stream_type_(stream_type), stream_(stream_type == StreamType::STDOUT ? &std::cout : &std::cerr),
      auto_flush_(true), thread_safe_(true), buffer_size_(0), show_timestamp_(true), show_level_(true),
      show_thread_id_(true), show_location_(true), show_function_(true), show_filename_(true),
      show_line_number_(true), show_milliseconds_(false), show_separator_(true), separator_(" "),
      show_newline_(true), show_prefix_(false), prefix_(""), show_suffix_(false), suffix_(""),
      show_progress_(false), progress_width_(20), show_stats_(false), stats_interval_(100),
      log_count_(0), last_stats_time_(std::chrono::steady_clock::now()) {
    colored_ = colored;
}

inline void ConsoleSink::log(const LogMessage& message) {
    std::lock_guard<std::mutex> lock(mutex_);
    if (colored_) {
        std::cout << "\033[1;32m" << message.message() << "\033[0m" << std::endl;
    } else {
        std::cout << message.message() << std::endl;
    }
}

inline void ConsoleSink::flush() {
    std::lock_guard<std::mutex> lock(mutex_);
    std::cout << std::flush;
}

inline void ConsoleSink::set_colored(bool enabled) {
    colored_ = enabled;
}

/**
 * @brief 彩色控制台输出目标
 * 专门用于彩色输出的控制台目标
 */
class ColoredConsoleSink : public ConsoleSink {
public:
    /**
     * @brief 构造函数
     * @param stream_type 输出流类型
     */
    explicit ColoredConsoleSink(StreamType stream_type = StreamType::STDOUT);
    
    /**
     * @brief 输出日志消息
     * @param message 日志消息
     */
    void log(const LogMessage& message) override;
    
    /**
     * @brief 设置是否启用背景色
     * @param enabled 是否启用
     */
    void set_show_background(bool enabled) { show_background_ = enabled; }
    
    /**
     * @brief 是否启用背景色
     * @return 是否启用
     */
    bool is_show_background() const { return show_background_; }
    
    /**
     * @brief 设置是否启用粗体
     * @param enabled 是否启用
     */
    void set_show_bold(bool enabled) { show_bold_ = enabled; }
    
    /**
     * @brief 是否启用粗体
     * @return 是否启用
     */
    bool is_show_bold() const { return show_bold_; }
    
    /**
     * @brief 设置是否启用下划线
     * @param enabled 是否启用
     */
    void set_show_underline(bool enabled) { show_underline_ = enabled; }
    
    /**
     * @brief 是否启用下划线
     * @return 是否启用
     */
    bool is_show_underline() const { return show_underline_; }
    
    /**
     * @brief 设置是否启用闪烁
     * @param enabled 是否启用
     */
    void set_show_blink(bool enabled) { show_blink_ = enabled; }
    
    /**
     * @brief 是否启用闪烁
     * @return 是否启用
     */
    bool is_show_blink() const { return show_blink_; }
    
    /**
     * @brief 设置是否启用反转
     * @param enabled 是否启用
     */
    void set_show_reverse(bool enabled) { show_reverse_ = enabled; }
    
    /**
     * @brief 是否启用反转
     * @return 是否启用
     */
    bool is_show_reverse() const { return show_reverse_; }
    
    /**
     * @brief 设置是否启用隐藏
     * @param enabled 是否启用
     */
    void set_show_hidden(bool enabled) { show_hidden_ = enabled; }
    
    /**
     * @brief 是否启用隐藏
     * @return 是否启用
     */
    bool is_show_hidden() const { return show_hidden_; }
    
    /**
     * @brief 设置是否启用彩虹模式
     * @param enabled 是否启用
     */
    void set_rainbow_mode(bool enabled) { rainbow_mode_ = enabled; }
    
    /**
     * @brief 是否启用彩虹模式
     * @return 是否启用
     */
    bool is_rainbow_mode() const { return rainbow_mode_; }
    
    /**
     * @brief 设置彩虹模式速度
     * @param speed 速度
     */
    void set_rainbow_speed(uint32_t speed) { rainbow_speed_ = speed; }
    
    /**
     * @brief 获取彩虹模式速度
     * @return 速度
     */
    uint32_t get_rainbow_speed() const { return rainbow_speed_; }
    
private:
    bool show_background_;                        ///< 是否显示背景色
    bool show_bold_;                              ///< 是否显示粗体
    bool show_underline_;                         ///< 是否显示下划线
    bool show_blink_;                             ///< 是否显示闪烁
    bool show_reverse_;                           ///< 是否显示反转
    bool show_hidden_;                            ///< 是否显示隐藏
    bool rainbow_mode_;                           ///< 是否启用彩虹模式
    uint32_t rainbow_speed_;                      ///< 彩虹模式速度
    std::atomic<uint32_t> rainbow_counter_;       ///< 彩虹模式计数器
    
    /**
     * @brief 获取彩虹颜色
     * @param index 颜色索引
     * @return 颜色代码
     */
    std::string get_rainbow_color(uint32_t index);
    
    /**
     * @brief 获取增强的颜色代码
     * @param level 日志级别
     * @return 颜色代码
     */
    std::string get_enhanced_color_code(LogLevel level);
};

} // namespace sugarlog 