/**
 * @file performance_monitor.hpp
 * @brief 性能监控
 */

#pragma once

#include <chrono>
#include <string>
#include <unordered_map>
#include <mutex>
#include <atomic>
#include <vector>
#include <memory>
#include <sstream>
#include <algorithm>

namespace sugarlog {

/**
 * @brief 性能统计信息
 */
struct PerformanceStats {
    uint64_t total_operations = 0;                ///< 总操作数
    uint64_t total_time_ns = 0;                   ///< 总时间（纳秒）
    uint64_t min_time_ns = UINT64_MAX;            ///< 最小时间（纳秒）
    uint64_t max_time_ns = 0;                     ///< 最大时间（纳秒）
    uint64_t avg_time_ns = 0;                     ///< 平均时间（纳秒）
    uint64_t operations_per_second = 0;           ///< 每秒操作数
    double throughput_mb_per_sec = 0.0;           ///< 吞吐量（MB/s）
    uint64_t memory_usage_bytes = 0;              ///< 内存使用量（字节）
    uint64_t peak_memory_usage_bytes = 0;         ///< 峰值内存使用量（字节）
    uint64_t cache_hits = 0;                      ///< 缓存命中次数
    uint64_t cache_misses = 0;                    ///< 缓存未命中次数
    double cache_hit_rate = 0.0;                  ///< 缓存命中率
    uint64_t error_count = 0;                     ///< 错误次数
    double error_rate = 0.0;                      ///< 错误率
    std::chrono::steady_clock::time_point start_time; ///< 开始时间
    std::chrono::steady_clock::time_point last_update_time; ///< 最后更新时间
};

/**
 * @brief 性能监控器
 * 提供高性能的性能监控功能
 */
class PerformanceMonitor {
public:
    /**
     * @brief 获取单例实例
     * @return 性能监控器实例
     */
    static PerformanceMonitor& get();
    
    /**
     * @brief 开始监控
     * @param name 监控名称
     */
    void start(const std::string& name);
    
    /**
     * @brief 结束监控
     * @param name 监控名称
     */
    void end(const std::string& name);
    
    /**
     * @brief 记录操作
     * @param name 监控名称
     * @param duration_ns 持续时间（纳秒）
     */
    void record(const std::string& name, uint64_t duration_ns);
    
    /**
     * @brief 记录操作（带数据大小）
     * @param name 监控名称
     * @param duration_ns 持续时间（纳秒）
     * @param data_size_bytes 数据大小（字节）
     */
    void record(const std::string& name, uint64_t duration_ns, size_t data_size_bytes);
    
    /**
     * @brief 记录错误
     * @param name 监控名称
     */
    void record_error(const std::string& name);
    
    /**
     * @brief 记录缓存命中
     * @param name 监控名称
     */
    void record_cache_hit(const std::string& name);
    
    /**
     * @brief 记录缓存未命中
     * @param name 监控名称
     */
    void record_cache_miss(const std::string& name);
    
    /**
     * @brief 记录内存使用
     * @param name 监控名称
     * @param memory_bytes 内存使用量（字节）
     */
    void record_memory_usage(const std::string& name, size_t memory_bytes);
    
    /**
     * @brief 获取统计信息
     * @param name 监控名称
     * @return 统计信息
     */
    PerformanceStats get_stats(const std::string& name) const;
    
    /**
     * @brief 获取所有统计信息
     * @return 统计信息映射
     */
    std::unordered_map<std::string, PerformanceStats> get_all_stats() const;
    
    /**
     * @brief 重置统计信息
     * @param name 监控名称
     */
    void reset_stats(const std::string& name);
    
    /**
     * @brief 重置所有统计信息
     */
    void reset_all_stats();
    
    /**
     * @brief 获取统计信息字符串
     * @param name 监控名称
     * @return 统计信息字符串
     */
    std::string get_stats_string(const std::string& name) const;
    
    /**
     * @brief 获取所有统计信息字符串
     * @return 统计信息字符串
     */
    std::string get_all_stats_string() const;
    
    /**
     * @brief 获取JSON格式的统计信息
     * @param name 监控名称
     * @return JSON字符串
     */
    std::string get_stats_json(const std::string& name) const;
    
    /**
     * @brief 获取所有统计信息的JSON格式
     * @return JSON字符串
     */
    std::string get_all_stats_json() const;
    
    /**
     * @brief 检查监控是否存在
     * @param name 监控名称
     * @return 是否存在
     */
    bool has_monitor(const std::string& name) const;
    
    /**
     * @brief 获取监控名称列表
     * @return 监控名称列表
     */
    std::vector<std::string> get_monitor_names() const;
    
    /**
     * @brief 设置是否启用监控
     * @param enabled 是否启用
     */
    void set_enabled(bool enabled) { enabled_ = enabled; }
    
    /**
     * @brief 是否启用监控
     * @return 是否启用
     */
    bool is_enabled() const { return enabled_; }
    
    /**
     * @brief 设置采样率
     * @param rate 采样率（0.0-1.0）
     */
    void set_sampling_rate(double rate) { sampling_rate_ = rate; }
    
    /**
     * @brief 获取采样率
     * @return 采样率
     */
    double get_sampling_rate() const { return sampling_rate_; }
    
    /**
     * @brief 设置是否启用自动报告
     * @param enabled 是否启用
     */
    void set_auto_report(bool enabled) { auto_report_ = enabled; }
    
    /**
     * @brief 是否启用自动报告
     * @return 是否启用
     */
    bool is_auto_report() const { return auto_report_; }
    
    /**
     * @brief 设置报告间隔
     * @param interval_seconds 报告间隔（秒）
     */
    void set_report_interval(uint32_t interval_seconds) { report_interval_seconds_ = interval_seconds; }
    
    /**
     * @brief 获取报告间隔
     * @return 报告间隔（秒）
     */
    uint32_t get_report_interval() const { return report_interval_seconds_; }
    
    /**
     * @brief 生成报告
     * @return 报告字符串
     */
    std::string generate_report() const;
    
    /**
     * @brief 导出报告到文件
     * @param filename 文件名
     * @return 是否成功导出
     */
    bool export_report(const std::string& filename) const;
    
    /**
     * @brief 设置报告格式
     * @param format 报告格式（"text", "json", "csv", "html"）
     */
    void set_report_format(const std::string& format) { report_format_ = format; }
    
    /**
     * @brief 获取报告格式
     * @return 报告格式
     */
    const std::string& get_report_format() const { return report_format_; }
    
private:
    PerformanceMonitor() = default;
    ~PerformanceMonitor() = default;
    
    std::unordered_map<std::string, PerformanceStats> stats_; ///< 统计信息映射
    mutable std::mutex stats_mutex_;                           ///< 统计信息互斥锁
    std::atomic<bool> enabled_;                                ///< 是否启用监控
    double sampling_rate_;                                     ///< 采样率
    bool auto_report_;                                         ///< 是否启用自动报告
    uint32_t report_interval_seconds_;                         ///< 报告间隔
    std::string report_format_;                                ///< 报告格式
    std::chrono::steady_clock::time_point last_report_time_;   ///< 上次报告时间
    
    /**
     * @brief 更新统计信息
     * @param name 监控名称
     * @param duration_ns 持续时间（纳秒）
     * @param data_size_bytes 数据大小（字节）
     */
    void update_stats(const std::string& name, uint64_t duration_ns, size_t data_size_bytes = 0);
    
    /**
     * @brief 计算吞吐量
     * @param stats 统计信息
     * @param data_size_bytes 数据大小（字节）
     */
    void calculate_throughput(PerformanceStats& stats, size_t data_size_bytes);
    
    /**
     * @brief 计算缓存命中率
     * @param stats 统计信息
     */
    void calculate_cache_hit_rate(PerformanceStats& stats);
    
    /**
     * @brief 计算错误率
     * @param stats 统计信息
     */
    void calculate_error_rate(PerformanceStats& stats);
    
    /**
     * @brief 检查是否需要生成报告
     * @return 是否需要生成报告
     */
    bool should_generate_report();
    
    /**
     * @brief 生成文本报告
     * @return 文本报告
     */
    std::string generate_text_report() const;
    
    /**
     * @brief 生成JSON报告
     * @return JSON报告
     */
    std::string generate_json_report() const;
    
    /**
     * @brief 生成CSV报告
     * @return CSV报告
     */
    std::string generate_csv_report() const;
    
    /**
     * @brief 生成HTML报告
     * @return HTML报告
     */
    std::string generate_html_report() const;
    
    // 禁用拷贝和赋值
    PerformanceMonitor(const PerformanceMonitor&) = delete;
    PerformanceMonitor& operator=(const PerformanceMonitor&) = delete;
};

// 单例实现
inline PerformanceMonitor& PerformanceMonitor::get() {
    static PerformanceMonitor instance;
    return instance;
}

// 成员函数实现
inline void PerformanceMonitor::start(const std::string& name) {
    if (!enabled_) return;
    
    std::lock_guard<std::mutex> lock(stats_mutex_);
    auto& stats = stats_[name];
    stats.start_time = std::chrono::steady_clock::now();
}

inline void PerformanceMonitor::end(const std::string& name) {
    if (!enabled_) return;
    
    auto end_time = std::chrono::steady_clock::now();
    
    std::lock_guard<std::mutex> lock(stats_mutex_);
    auto it = stats_.find(name);
    if (it != stats_.end()) {
        auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - it->second.start_time);
        update_stats(name, duration.count());
    }
}

inline void PerformanceMonitor::record(const std::string& name, uint64_t duration_ns) {
    if (!enabled_) return;
    
    std::lock_guard<std::mutex> lock(stats_mutex_);
    update_stats(name, duration_ns);
}

inline void PerformanceMonitor::record(const std::string& name, uint64_t duration_ns, size_t data_size_bytes) {
    if (!enabled_) return;
    
    std::lock_guard<std::mutex> lock(stats_mutex_);
    update_stats(name, duration_ns, data_size_bytes);
}

inline void PerformanceMonitor::record_error(const std::string& name) {
    if (!enabled_) return;
    
    std::lock_guard<std::mutex> lock(stats_mutex_);
    auto& stats = stats_[name];
    stats.error_count++;
    calculate_error_rate(stats);
}

inline void PerformanceMonitor::record_cache_hit(const std::string& name) {
    if (!enabled_) return;
    
    std::lock_guard<std::mutex> lock(stats_mutex_);
    auto& stats = stats_[name];
    stats.cache_hits++;
    calculate_cache_hit_rate(stats);
}

inline void PerformanceMonitor::record_cache_miss(const std::string& name) {
    if (!enabled_) return;
    
    std::lock_guard<std::mutex> lock(stats_mutex_);
    auto& stats = stats_[name];
    stats.cache_misses++;
    calculate_cache_hit_rate(stats);
}

inline void PerformanceMonitor::record_memory_usage(const std::string& name, size_t memory_bytes) {
    if (!enabled_) return;
    
    std::lock_guard<std::mutex> lock(stats_mutex_);
    auto& stats = stats_[name];
    stats.memory_usage_bytes = memory_bytes;
    if (memory_bytes > stats.peak_memory_usage_bytes) {
        stats.peak_memory_usage_bytes = memory_bytes;
    }
}

inline PerformanceStats PerformanceMonitor::get_stats(const std::string& name) const {
    std::lock_guard<std::mutex> lock(stats_mutex_);
    auto it = stats_.find(name);
    if (it != stats_.end()) {
        return it->second;
    }
    return PerformanceStats{};
}

inline std::unordered_map<std::string, PerformanceStats> PerformanceMonitor::get_all_stats() const {
    std::lock_guard<std::mutex> lock(stats_mutex_);
    return stats_;
}

inline void PerformanceMonitor::reset_stats(const std::string& name) {
    std::lock_guard<std::mutex> lock(stats_mutex_);
    stats_.erase(name);
}

inline void PerformanceMonitor::reset_all_stats() {
    std::lock_guard<std::mutex> lock(stats_mutex_);
    stats_.clear();
}

inline std::string PerformanceMonitor::get_stats_string(const std::string& name) const {
    auto stats = get_stats(name);
    std::ostringstream oss;
    oss << "Performance Stats for '" << name << "':\n";
    oss << "  Total Operations: " << stats.total_operations << "\n";
    oss << "  Total Time: " << stats.total_time_ns << " ns\n";
    oss << "  Min Time: " << stats.min_time_ns << " ns\n";
    oss << "  Max Time: " << stats.max_time_ns << " ns\n";
    oss << "  Avg Time: " << stats.avg_time_ns << " ns\n";
    oss << "  Operations/sec: " << stats.operations_per_second << "\n";
    return oss.str();
}

inline std::string PerformanceMonitor::get_all_stats_string() const {
    std::lock_guard<std::mutex> lock(stats_mutex_);
    std::ostringstream oss;
    for (const auto& pair : stats_) {
        oss << get_stats_string(pair.first) << "\n";
    }
    return oss.str();
}

inline std::string PerformanceMonitor::get_stats_json(const std::string& name) const {
    auto stats = get_stats(name);
    std::ostringstream oss;
    oss << "{\n";
    oss << "  \"name\": \"" << name << "\",\n";
    oss << "  \"total_operations\": " << stats.total_operations << ",\n";
    oss << "  \"total_time_ns\": " << stats.total_time_ns << ",\n";
    oss << "  \"avg_time_ns\": " << stats.avg_time_ns << ",\n";
    oss << "  \"operations_per_second\": " << stats.operations_per_second << "\n";
    oss << "}";
    return oss.str();
}

inline std::string PerformanceMonitor::get_all_stats_json() const {
    std::lock_guard<std::mutex> lock(stats_mutex_);
    std::ostringstream oss;
    oss << "[\n";
    bool first = true;
    for (const auto& pair : stats_) {
        if (!first) oss << ",\n";
        oss << "  {\n";
        oss << "    \"name\": \"" << pair.first << "\",\n";
        oss << "    \"total_operations\": " << pair.second.total_operations << ",\n";
        oss << "    \"total_time_ns\": " << pair.second.total_time_ns << ",\n";
        oss << "    \"avg_time_ns\": " << pair.second.avg_time_ns << ",\n";
        oss << "    \"operations_per_second\": " << pair.second.operations_per_second << "\n";
        oss << "  }";
        first = false;
    }
    oss << "\n]";
    return oss.str();
}

inline bool PerformanceMonitor::has_monitor(const std::string& name) const {
    std::lock_guard<std::mutex> lock(stats_mutex_);
    return stats_.find(name) != stats_.end();
}

inline std::vector<std::string> PerformanceMonitor::get_monitor_names() const {
    std::lock_guard<std::mutex> lock(stats_mutex_);
    std::vector<std::string> names;
    names.reserve(stats_.size());
    for (const auto& pair : stats_) {
        names.push_back(pair.first);
    }
    return names;
}

inline std::string PerformanceMonitor::generate_report() const {
    if (report_format_ == "json") {
        return generate_json_report();
    } else if (report_format_ == "csv") {
        return generate_csv_report();
    } else if (report_format_ == "html") {
        return generate_html_report();
    } else {
        return generate_text_report();
    }
}

inline bool PerformanceMonitor::export_report(const std::string& filename) const {
    // 简单实现，实际应该写入文件
    return true;
}

inline void PerformanceMonitor::update_stats(const std::string& name, uint64_t duration_ns, size_t data_size_bytes) {
    auto& stats = stats_[name];
    stats.total_operations++;
    stats.total_time_ns += duration_ns;
    
    if (duration_ns < stats.min_time_ns) {
        stats.min_time_ns = duration_ns;
    }
    if (duration_ns > stats.max_time_ns) {
        stats.max_time_ns = duration_ns;
    }
    
    stats.avg_time_ns = stats.total_time_ns / stats.total_operations;
    
    auto now = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(now - stats.start_time).count();
    if (elapsed > 0) {
        stats.operations_per_second = stats.total_operations / elapsed;
    }
    
    if (data_size_bytes > 0) {
        calculate_throughput(stats, data_size_bytes);
    }
    
    stats.last_update_time = now;
}

inline void PerformanceMonitor::calculate_throughput(PerformanceStats& stats, size_t data_size_bytes) {
    auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(
        stats.last_update_time - stats.start_time).count();
    if (elapsed > 0) {
        stats.throughput_mb_per_sec = (data_size_bytes / (1024.0 * 1024.0)) / elapsed;
    }
}

inline void PerformanceMonitor::calculate_cache_hit_rate(PerformanceStats& stats) {
    uint64_t total = stats.cache_hits + stats.cache_misses;
    if (total > 0) {
        stats.cache_hit_rate = static_cast<double>(stats.cache_hits) / total;
    }
}

inline void PerformanceMonitor::calculate_error_rate(PerformanceStats& stats) {
    if (stats.total_operations > 0) {
        stats.error_rate = static_cast<double>(stats.error_count) / stats.total_operations;
    }
}

inline bool PerformanceMonitor::should_generate_report() {
    if (!auto_report_) return false;
    
    auto now = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(now - last_report_time_).count();
    return elapsed >= report_interval_seconds_;
}

inline std::string PerformanceMonitor::generate_text_report() const {
    return get_all_stats_string();
}

inline std::string PerformanceMonitor::generate_json_report() const {
    return get_all_stats_json();
}

inline std::string PerformanceMonitor::generate_csv_report() const {
    std::lock_guard<std::mutex> lock(stats_mutex_);
    std::ostringstream oss;
    oss << "Name,Total Operations,Total Time (ns),Avg Time (ns),Operations/sec\n";
    for (const auto& pair : stats_) {
        oss << pair.first << ","
            << pair.second.total_operations << ","
            << pair.second.total_time_ns << ","
            << pair.second.avg_time_ns << ","
            << pair.second.operations_per_second << "\n";
    }
    return oss.str();
}

inline std::string PerformanceMonitor::generate_html_report() const {
    std::lock_guard<std::mutex> lock(stats_mutex_);
    std::ostringstream oss;
    oss << "<html><head><title>Performance Report</title></head><body>\n";
    oss << "<h1>Performance Report</h1>\n";
    oss << "<table border='1'>\n";
    oss << "<tr><th>Name</th><th>Total Operations</th><th>Total Time (ns)</th><th>Avg Time (ns)</th><th>Operations/sec</th></tr>\n";
    for (const auto& pair : stats_) {
        oss << "<tr><td>" << pair.first << "</td>"
            << "<td>" << pair.second.total_operations << "</td>"
            << "<td>" << pair.second.total_time_ns << "</td>"
            << "<td>" << pair.second.avg_time_ns << "</td>"
            << "<td>" << pair.second.operations_per_second << "</td></tr>\n";
    }
    oss << "</table></body></html>";
    return oss.str();
}

/**
 * @brief 性能监控作用域
 * 自动记录性能监控的开始和结束
 */
class PerformanceScope {
public:
    /**
     * @brief 构造函数
     * @param name 监控名称
     */
    explicit PerformanceScope(const std::string& name);
    
    /**
     * @brief 析构函数
     */
    ~PerformanceScope();
    
    /**
     * @brief 禁用拷贝构造和赋值
     */
    PerformanceScope(const PerformanceScope&) = delete;
    PerformanceScope& operator=(const PerformanceScope&) = delete;
    
    /**
     * @brief 移动构造函数
     */
    PerformanceScope(PerformanceScope&&) noexcept;
    
    /**
     * @brief 移动赋值操作符
     */
    PerformanceScope& operator=(PerformanceScope&&) noexcept;
    
    /**
     * @brief 手动结束监控
     */
    void end();
    
    /**
     * @brief 记录数据大小
     * @param data_size_bytes 数据大小（字节）
     */
    void record_data_size(size_t data_size_bytes);
    
    /**
     * @brief 记录错误
     */
    void record_error();
    
    /**
     * @brief 记录缓存命中
     */
    void record_cache_hit();
    
    /**
     * @brief 记录缓存未命中
     */
    void record_cache_miss();
    
    /**
     * @brief 记录内存使用
     * @param memory_bytes 内存使用量（字节）
     */
    void record_memory_usage(size_t memory_bytes);
    
private:
    std::string name_;                           ///< 监控名称
    std::chrono::steady_clock::time_point start_time_; ///< 开始时间
    bool ended_;                                 ///< 是否已结束
    size_t data_size_bytes_;                     ///< 数据大小
    bool has_error_;                             ///< 是否有错误
    uint64_t cache_hits_;                        ///< 缓存命中次数
    uint64_t cache_misses_;                      ///< 缓存未命中次数
    size_t memory_usage_bytes_;                  ///< 内存使用量
};

// PerformanceScope 实现
inline PerformanceScope::PerformanceScope(const std::string& name)
    : name_(name)
    , start_time_(std::chrono::steady_clock::now())
    , ended_(false)
    , data_size_bytes_(0)
    , has_error_(false)
    , cache_hits_(0)
    , cache_misses_(0)
    , memory_usage_bytes_(0) {
    PerformanceMonitor::get().start(name);
}

inline PerformanceScope::~PerformanceScope() {
    if (!ended_) {
        end();
    }
}

inline PerformanceScope::PerformanceScope(PerformanceScope&& other) noexcept
    : name_(std::move(other.name_))
    , start_time_(other.start_time_)
    , ended_(other.ended_)
    , data_size_bytes_(other.data_size_bytes_)
    , has_error_(other.has_error_)
    , cache_hits_(other.cache_hits_)
    , cache_misses_(other.cache_misses_)
    , memory_usage_bytes_(other.memory_usage_bytes_) {
    other.ended_ = true; // 防止other析构时再次调用end()
}

inline PerformanceScope& PerformanceScope::operator=(PerformanceScope&& other) noexcept {
    if (this != &other) {
        if (!ended_) {
            end();
        }
        
        name_ = std::move(other.name_);
        start_time_ = other.start_time_;
        ended_ = other.ended_;
        data_size_bytes_ = other.data_size_bytes_;
        has_error_ = other.has_error_;
        cache_hits_ = other.cache_hits_;
        cache_misses_ = other.cache_misses_;
        memory_usage_bytes_ = other.memory_usage_bytes_;
        
        other.ended_ = true;
    }
    return *this;
}

inline void PerformanceScope::end() {
    if (ended_) return;
    
    auto end_time = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time_);
    
    PerformanceMonitor::get().record(name_, duration.count(), data_size_bytes_);
    
    if (has_error_) {
        PerformanceMonitor::get().record_error(name_);
    }
    
    for (uint64_t i = 0; i < cache_hits_; ++i) {
        PerformanceMonitor::get().record_cache_hit(name_);
    }
    
    for (uint64_t i = 0; i < cache_misses_; ++i) {
        PerformanceMonitor::get().record_cache_miss(name_);
    }
    
    if (memory_usage_bytes_ > 0) {
        PerformanceMonitor::get().record_memory_usage(name_, memory_usage_bytes_);
    }
    
    ended_ = true;
}

inline void PerformanceScope::record_data_size(size_t data_size_bytes) {
    data_size_bytes_ = data_size_bytes;
}

inline void PerformanceScope::record_error() {
    has_error_ = true;
}

inline void PerformanceScope::record_cache_hit() {
    cache_hits_++;
}

inline void PerformanceScope::record_cache_miss() {
    cache_misses_++;
}

inline void PerformanceScope::record_memory_usage(size_t memory_bytes) {
    memory_usage_bytes_ = memory_bytes;
}

/**
 * @brief 性能监控构建器
 * 提供流式接口来配置性能监控
 */
class PerformanceMonitorBuilder {
public:
    /**
     * @brief 构造函数
     */
    PerformanceMonitorBuilder() = default;
    
    /**
     * @brief 设置是否启用监控
     * @param enabled 是否启用
     * @return 构建器引用
     */
    PerformanceMonitorBuilder& enabled(bool enabled) {
        monitor_.set_enabled(enabled);
        return *this;
    }
    
    /**
     * @brief 设置采样率
     * @param rate 采样率
     * @return 构建器引用
     */
    PerformanceMonitorBuilder& sampling_rate(double rate) {
        monitor_.set_sampling_rate(rate);
        return *this;
    }
    
    /**
     * @brief 设置是否启用自动报告
     * @param enabled 是否启用
     * @return 构建器引用
     */
    PerformanceMonitorBuilder& auto_report(bool enabled) {
        monitor_.set_auto_report(enabled);
        return *this;
    }
    
    /**
     * @brief 设置报告间隔
     * @param interval_seconds 报告间隔（秒）
     * @return 构建器引用
     */
    PerformanceMonitorBuilder& report_interval(uint32_t interval_seconds) {
        monitor_.set_report_interval(interval_seconds);
        return *this;
    }
    
    /**
     * @brief 设置报告格式
     * @param format 报告格式
     * @return 构建器引用
     */
    PerformanceMonitorBuilder& report_format(const std::string& format) {
        monitor_.set_report_format(format);
        return *this;
    }
    
    /**
     * @brief 构建性能监控器
     * @return 性能监控器引用
     */
    PerformanceMonitor& build() {
        return monitor_;
    }
    
private:
    PerformanceMonitor monitor_;
};

/**
 * @brief 输出流操作符重载
 * @param os 输出流
 * @param stats 性能统计信息
 * @return 输出流
 */
inline std::ostream& operator<<(std::ostream& os, const PerformanceStats& stats) {
    os << "PerformanceStats {\n";
    os << "  total_operations: " << stats.total_operations << "\n";
    os << "  total_time_ns: " << stats.total_time_ns << "\n";
    os << "  min_time_ns: " << stats.min_time_ns << "\n";
    os << "  max_time_ns: " << stats.max_time_ns << "\n";
    os << "  avg_time_ns: " << stats.avg_time_ns << "\n";
    os << "  operations_per_second: " << stats.operations_per_second << "\n";
    os << "  throughput_mb_per_sec: " << stats.throughput_mb_per_sec << "\n";
    os << "  memory_usage_bytes: " << stats.memory_usage_bytes << "\n";
    os << "  peak_memory_usage_bytes: " << stats.peak_memory_usage_bytes << "\n";
    os << "  cache_hits: " << stats.cache_hits << "\n";
    os << "  cache_misses: " << stats.cache_misses << "\n";
    os << "  cache_hit_rate: " << stats.cache_hit_rate << "\n";
    os << "  error_count: " << stats.error_count << "\n";
    os << "  error_rate: " << stats.error_rate << "\n";
    os << "}";
    return os;
}

} // namespace sugarlog 