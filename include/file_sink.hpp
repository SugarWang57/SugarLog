/**
 * @file file_sink.hpp
 * @brief 文件输出目标
 */

#pragma once

#include "log_sink.hpp"
#include <fstream>
#include <filesystem>
#include <memory>
#include <mutex>

namespace sugarlog {

/**
 * @brief 文件轮转策略
 */
enum class RotationPolicy {
    NONE,           ///< 不轮转
    SIZE,           ///< 按大小轮转
    TIME,           ///< 按时间轮转
    SIZE_AND_TIME   ///< 按大小和时间轮转
};

/**
 * @brief 文件压缩策略
 */
enum class CompressionPolicy {
    NONE,           ///< 不压缩
    GZIP,           ///< GZIP压缩
    BZIP2,          ///< BZIP2压缩
    LZ4,            ///< LZ4压缩
    ZSTD            ///< ZSTD压缩
};

/**
 * @brief 文件输出目标配置
 */
struct FileSinkConfig {
    std::string filename;                         ///< 文件名
    std::string directory;                        ///< 目录
    std::string pattern = "%Y-%m-%d";            ///< 文件名模式
    RotationPolicy rotation_policy = RotationPolicy::SIZE; ///< 轮转策略
    size_t max_file_size = 10 * 1024 * 1024;     ///< 最大文件大小（字节）
    uint32_t max_files = 10;                      ///< 最大文件数量
    uint32_t rotation_interval_hours = 24;        ///< 轮转间隔（小时）
    CompressionPolicy compression_policy = CompressionPolicy::GZIP; ///< 压缩策略
    bool auto_flush = true;                       ///< 是否自动刷新
    bool create_directories = true;               ///< 是否自动创建目录
    bool append = false;                          ///< 是否追加模式
    bool binary = false;                          ///< 是否二进制模式
    size_t buffer_size = 8192;                    ///< 缓冲区大小
    bool thread_safe = true;                      ///< 是否线程安全
    bool enable_rotation = true;                  ///< 是否启用轮转
    bool enable_compression = true;               ///< 是否启用压缩
    bool enable_backup = true;                    ///< 是否启用备份
    std::string backup_suffix = ".bak";           ///< 备份文件后缀
    bool enable_timestamp = true;                 ///< 是否启用时间戳
    std::string timestamp_format = "%Y%m%d_%H%M%S"; ///< 时间戳格式
    bool enable_checksum = false;                 ///< 是否启用校验和
    std::string checksum_algorithm = "MD5";       ///< 校验和算法
    bool enable_encryption = false;               ///< 是否启用加密
    std::string encryption_key;                   ///< 加密密钥
    std::string encryption_algorithm = "AES256";  ///< 加密算法
};

/**
 * @brief 文件输出目标
 * 将日志输出到文件
 */
class FileSink : public LogSink {
public:
    /**
     * @brief 构造函数
     * @param config 配置参数
     */
    explicit FileSink(const FileSinkConfig& config = FileSinkConfig{});
    
    /**
     * @brief 析构函数
     */
    ~FileSink() override;
    
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
     * @brief 打开文件
     * @return 是否成功打开
     */
    bool open();
    
    /**
     * @brief 关闭文件
     */
    void close();
    
    /**
     * @brief 检查文件是否打开
     * @return 是否打开
     */
    bool is_open() const;
    
    /**
     * @brief 获取当前文件名
     * @return 当前文件名
     */
    std::string get_current_filename() const;
    
    /**
     * @brief 获取文件大小
     * @return 文件大小（字节）
     */
    size_t get_file_size() const;
    
    /**
     * @brief 设置文件名
     * @param filename 文件名
     */
    void set_filename(const std::string& filename);
    
    /**
     * @brief 获取文件名
     * @return 文件名
     */
    const std::string& get_filename() const { return config_.filename; }
    
    /**
     * @brief 设置目录
     * @param directory 目录
     */
    void set_directory(const std::string& directory);
    
    /**
     * @brief 获取目录
     * @return 目录
     */
    const std::string& get_directory() const { return config_.directory; }
    
    /**
     * @brief 设置轮转策略
     * @param policy 轮转策略
     */
    void set_rotation_policy(RotationPolicy policy);
    
    /**
     * @brief 获取轮转策略
     * @return 轮转策略
     */
    RotationPolicy get_rotation_policy() const { return config_.rotation_policy; }
    
    /**
     * @brief 设置最大文件大小
     * @param size 最大文件大小（字节）
     */
    void set_max_file_size(size_t size);
    
    /**
     * @brief 获取最大文件大小
     * @return 最大文件大小（字节）
     */
    size_t get_max_file_size() const { return config_.max_file_size; }
    
    /**
     * @brief 设置最大文件数量
     * @param count 最大文件数量
     */
    void set_max_files(uint32_t count);
    
    /**
     * @brief 获取最大文件数量
     * @return 最大文件数量
     */
    uint32_t get_max_files() const { return config_.max_files; }
    
    /**
     * @brief 设置轮转间隔
     * @param hours 轮转间隔（小时）
     */
    void set_rotation_interval(uint32_t hours);
    
    /**
     * @brief 获取轮转间隔
     * @return 轮转间隔（小时）
     */
    uint32_t get_rotation_interval() const { return config_.rotation_interval_hours; }
    
    /**
     * @brief 设置压缩策略
     * @param policy 压缩策略
     */
    void set_compression_policy(CompressionPolicy policy);
    
    /**
     * @brief 获取压缩策略
     * @return 压缩策略
     */
    CompressionPolicy get_compression_policy() const { return config_.compression_policy; }
    
    /**
     * @brief 设置是否自动刷新
     * @param enabled 是否启用
     */
    void set_auto_flush(bool enabled);
    
    /**
     * @brief 是否自动刷新
     * @return 是否启用
     */
    bool is_auto_flush() const { return config_.auto_flush; }
    
    /**
     * @brief 设置是否追加模式
     * @param enabled 是否启用
     */
    void set_append(bool enabled);
    
    /**
     * @brief 是否追加模式
     * @return 是否启用
     */
    bool is_append() const { return config_.append; }
    
    /**
     * @brief 设置是否二进制模式
     * @param enabled 是否启用
     */
    void set_binary(bool enabled);
    
    /**
     * @brief 是否二进制模式
     * @return 是否启用
     */
    bool is_binary() const { return config_.binary; }
    
    /**
     * @brief 设置缓冲区大小
     * @param size 缓冲区大小
     */
    void set_buffer_size(size_t size);
    
    /**
     * @brief 获取缓冲区大小
     * @return 缓冲区大小
     */
    size_t get_buffer_size() const { return config_.buffer_size; }
    
    /**
     * @brief 强制轮转文件
     * @return 是否成功轮转
     */
    bool rotate();
    
    /**
     * @brief 压缩文件
     * @param filename 要压缩的文件名
     * @return 是否成功压缩
     */
    bool compress_file(const std::string& filename);
    
    /**
     * @brief 解压文件
     * @param filename 要解压的文件名
     * @return 是否成功解压
     */
    bool decompress_file(const std::string& filename);
    
    /**
     * @brief 清理旧文件
     * @return 清理的文件数量
     */
    size_t cleanup_old_files();
    
    /**
     * @brief 获取文件列表
     * @return 文件列表
     */
    std::vector<std::string> get_file_list() const;
    
    /**
     * @brief 获取文件统计信息
     * @return 统计信息字符串
     */
    std::string get_file_stats() const;
    
    /**
     * @brief 设置是否启用备份
     * @param enabled 是否启用
     */
    void set_enable_backup(bool enabled) { config_.enable_backup = enabled; }
    
    /**
     * @brief 是否启用备份
     * @return 是否启用
     */
    bool is_enable_backup() const { return config_.enable_backup; }
    
    /**
     * @brief 设置备份文件后缀
     * @param suffix 后缀
     */
    void set_backup_suffix(const std::string& suffix) { config_.backup_suffix = suffix; }
    
    /**
     * @brief 获取备份文件后缀
     * @return 后缀
     */
    const std::string& get_backup_suffix() const { return config_.backup_suffix; }
    
    /**
     * @brief 设置是否启用校验和
     * @param enabled 是否启用
     */
    void set_enable_checksum(bool enabled) { config_.enable_checksum = enabled; }
    
    /**
     * @brief 是否启用校验和
     * @return 是否启用
     */
    bool is_enable_checksum() const { return config_.enable_checksum; }
    
    /**
     * @brief 设置校验和算法
     * @param algorithm 算法名称
     */
    void set_checksum_algorithm(const std::string& algorithm) { config_.checksum_algorithm = algorithm; }
    
    /**
     * @brief 获取校验和算法
     * @return 算法名称
     */
    const std::string& get_checksum_algorithm() const { return config_.checksum_algorithm; }
    
    /**
     * @brief 设置是否启用加密
     * @param enabled 是否启用
     */
    void set_enable_encryption(bool enabled) { config_.enable_encryption = enabled; }
    
    /**
     * @brief 是否启用加密
     * @return 是否启用
     */
    bool is_enable_encryption() const { return config_.enable_encryption; }
    
    /**
     * @brief 设置加密密钥
     * @param key 加密密钥
     */
    void set_encryption_key(const std::string& key) { config_.encryption_key = key; }
    
    /**
     * @brief 获取加密密钥
     * @return 加密密钥
     */
    const std::string& get_encryption_key() const { return config_.encryption_key; }
    
    /**
     * @brief 设置加密算法
     * @param algorithm 算法名称
     */
    void set_encryption_algorithm(const std::string& algorithm) { config_.encryption_algorithm = algorithm; }
    
    /**
     * @brief 获取加密算法
     * @return 算法名称
     */
    const std::string& get_encryption_algorithm() const { return config_.encryption_algorithm; }
    
protected:
    FileSinkConfig config_;                       ///< 配置参数
    std::unique_ptr<std::ofstream> file_;         ///< 文件流
    std::string current_filename_;                ///< 当前文件名
    std::chrono::system_clock::time_point last_rotation_time_; ///< 上次轮转时间
    size_t current_file_size_;                    ///< 当前文件大小
    mutable std::mutex file_mutex_;               ///< 文件互斥锁
    std::ofstream ofs_;
    std::mutex mutex_;
    
    /**
     * @brief 生成文件名
     * @return 生成的文件名
     */
    std::string generate_filename();
    
    /**
     * @brief 检查是否需要轮转
     * @return 是否需要轮转
     */
    bool should_rotate();
    
    /**
     * @brief 执行文件轮转
     * @return 是否成功轮转
     */
    bool perform_rotation();
    
    /**
     * @brief 创建备份文件
     * @param filename 原文件名
     * @return 备份文件名
     */
    std::string create_backup(const std::string& filename);
    
    /**
     * @brief 计算文件校验和
     * @param filename 文件名
     * @return 校验和字符串
     */
    std::string calculate_checksum(const std::string& filename);
    
    /**
     * @brief 加密文件
     * @param input_filename 输入文件名
     * @param output_filename 输出文件名
     * @return 是否成功加密
     */
    bool encrypt_file(const std::string& input_filename, const std::string& output_filename);
    
    /**
     * @brief 解密文件
     * @param input_filename 输入文件名
     * @param output_filename 输出文件名
     * @return 是否成功解密
     */
    bool decrypt_file(const std::string& input_filename, const std::string& output_filename);
    
    /**
     * @brief 确保目录存在
     * @param directory 目录路径
     * @return 是否成功
     */
    bool ensure_directory_exists(const std::string& directory);
    
    /**
     * @brief 获取文件扩展名
     * @param filename 文件名
     * @return 扩展名
     */
    std::string get_file_extension(const std::string& filename);
    
    /**
     * @brief 获取压缩文件扩展名
     * @return 压缩文件扩展名
     */
    std::string get_compression_extension();
    
    /**
     * @brief 写入数据到文件
     * @param data 数据
     * @param size 数据大小
     * @return 是否成功写入
     */
    bool write_to_file(const char* data, size_t size);
    
    /**
     * @brief 写入字符串到文件
     * @param str 字符串
     * @return 是否成功写入
     */
    bool write_to_file(const std::string& str);
};

// FileSink 实现
inline FileSink::FileSink(const FileSinkConfig& config) : config_(config) {}

inline void FileSink::log(const LogMessage& message) {
    std::lock_guard<std::mutex> lock(mutex_);
    if (!ofs_.is_open()) {
        ofs_.open(config_.filename, std::ios::app);
    }
    if (ofs_.is_open()) {
        ofs_ << message.message() << std::endl;
    }
}

inline void FileSink::flush() {
    std::lock_guard<std::mutex> lock(mutex_);
    if (ofs_.is_open()) {
        ofs_ << std::flush;
    }
}

// FileSink 析构函数实现
inline FileSink::~FileSink() = default;

/**
 * @brief 滚动文件输出目标
 * 支持按大小和时间滚动的文件输出
 */
class RollingFileSink : public FileSink {
public:
    /**
     * @brief 构造函数
     * @param filename 文件名
     * @param max_size 最大文件大小
     * @param max_files 最大文件数量
     */
    RollingFileSink(const std::string& filename, 
                   size_t max_size = 10 * 1024 * 1024,
                   uint32_t max_files = 10);
    
    /**
     * @brief 输出日志消息
     * @param message 日志消息
     */
    void log(const LogMessage& message) override;
    
    /**
     * @brief 设置滚动策略
     * @param policy 滚动策略
     */
    void set_rolling_policy(RotationPolicy policy);
    
    /**
     * @brief 获取滚动策略
     * @return 滚动策略
     */
    RotationPolicy get_rolling_policy() const;
    
    /**
     * @brief 设置滚动间隔
     * @param hours 滚动间隔（小时）
     */
    void set_rolling_interval(uint32_t hours);
    
    /**
     * @brief 获取滚动间隔
     * @return 滚动间隔（小时）
     */
    uint32_t get_rolling_interval() const;
    
    /**
     * @brief 强制滚动
     * @return 是否成功滚动
     */
    bool roll();
    
private:
    /**
     * @brief 检查是否需要滚动
     * @return 是否需要滚动
     */
    bool should_roll();
    
    /**
     * @brief 执行滚动
     * @return 是否成功滚动
     */
    bool perform_roll();
    
    /**
     * @brief 生成滚动文件名
     * @param index 文件索引
     * @return 滚动文件名
     */
    std::string generate_rolling_filename(uint32_t index);
};

/**
 * @brief 每日文件输出目标
 * 按日期自动创建新文件的输出目标
 */
class DailyFileSink : public FileSink {
public:
    /**
     * @brief 构造函数
     * @param filename 文件名
     * @param pattern 日期模式
     */
    DailyFileSink(const std::string& filename, 
                  const std::string& pattern = "%Y-%m-%d");
    
    /**
     * @brief 输出日志消息
     * @param message 日志消息
     */
    void log(const LogMessage& message) override;
    
    /**
     * @brief 设置日期模式
     * @param pattern 日期模式
     */
    void set_date_pattern(const std::string& pattern);
    
    /**
     * @brief 获取日期模式
     * @return 日期模式
     */
    const std::string& get_date_pattern() const { return date_pattern_; }
    
    /**
     * @brief 设置是否保留旧文件
     * @param enabled 是否启用
     */
    void set_keep_old_files(bool enabled) { keep_old_files_ = enabled; }
    
    /**
     * @brief 是否保留旧文件
     * @return 是否启用
     */
    bool is_keep_old_files() const { return keep_old_files_; }
    
    /**
     * @brief 设置保留天数
     * @param days 保留天数
     */
    void set_keep_days(uint32_t days) { keep_days_ = days; }
    
    /**
     * @brief 获取保留天数
     * @return 保留天数
     */
    uint32_t get_keep_days() const { return keep_days_; }
    
private:
    std::string date_pattern_;                    ///< 日期模式
    std::string current_date_;                    ///< 当前日期
    bool keep_old_files_;                         ///< 是否保留旧文件
    uint32_t keep_days_;                          ///< 保留天数
    
    /**
     * @brief 检查是否需要切换文件
     * @return 是否需要切换
     */
    bool should_switch_file();
    
    /**
     * @brief 切换文件
     * @return 是否成功切换
     */
    bool switch_file();
    
    /**
     * @brief 清理旧文件
     */
    void cleanup_old_files();
    
    /**
     * @brief 生成日期文件名
     * @return 日期文件名
     */
    std::string generate_date_filename();
};

} // namespace sugarlog 