/**
 * @file memory_pool.hpp
 * @brief 高性能内存池
 */

#pragma once

#include <vector>
#include <mutex>
#include <memory>
#include <cstddef>
#include <cstdint>
#include <unordered_map>

namespace sugarlog {

/**
 * @brief 内存块结构
 */
struct MemoryBlock {
    MemoryBlock* next;    ///< 下一个内存块
    uint8_t data[];       ///< 数据区域
};

/**
 * @brief 内存池类
 * 提供高性能的内存分配和回收
 */
class MemoryPool {
public:
    /**
     * @brief 构造函数
     * @param block_size 内存块大小
     * @param initial_blocks 初始内存块数量
     * @param max_blocks 最大内存块数量
     */
    explicit MemoryPool(size_t block_size, 
                       size_t initial_blocks = 10,
                       size_t max_blocks = 1000);
    
    /**
     * @brief 析构函数
     */
    ~MemoryPool();
    
    /**
     * @brief 分配内存
     * @return 分配的内存指针
     */
    void* allocate();
    
    /**
     * @brief 释放内存
     * @param ptr 要释放的内存指针
     */
    void deallocate(void* ptr);
    
    /**
     * @brief 获取内存块大小
     * @return 内存块大小
     */
    size_t get_block_size() const { return block_size_; }
    
    /**
     * @brief 获取当前内存块数量
     * @return 当前内存块数量
     */
    size_t get_current_blocks() const { return current_blocks_; }
    
    /**
     * @brief 获取最大内存块数量
     * @return 最大内存块数量
     */
    size_t get_max_blocks() const { return max_blocks_; }
    
    /**
     * @brief 获取可用内存块数量
     * @return 可用内存块数量
     */
    size_t get_available_blocks() const;
    
    /**
     * @brief 清空内存池
     */
    void clear();
    
    /**
     * @brief 预分配内存块
     * @param count 要预分配的块数
     */
    void preallocate(size_t count);
    
private:
    size_t block_size_;                   ///< 内存块大小
    size_t max_blocks_;                   ///< 最大内存块数量
    size_t current_blocks_;               ///< 当前内存块数量
    MemoryBlock* free_list_;              ///< 空闲内存块链表
    std::vector<MemoryBlock*> allocated_blocks_; ///< 已分配的内存块列表
    mutable std::mutex mutex_;            ///< 互斥锁
    
    /**
     * @brief 创建新的内存块
     * @return 新创建的内存块
     */
    MemoryBlock* create_block();
    
    /**
     * @brief 销毁内存块
     * @param block 要销毁的内存块
     */
    void destroy_block(MemoryBlock* block);
    
    /**
     * @brief 扩展内存池
     * @param count 要扩展的块数
     */
    void expand(size_t count);
};

/**
 * @brief 多级内存池
 * 支持多种大小的内存块分配
 */
class MultiLevelMemoryPool {
public:
    /**
     * @brief 构造函数
     * @param max_block_size 最大内存块大小
     * @param growth_factor 增长因子
     */
    explicit MultiLevelMemoryPool(size_t max_block_size = 4096,
                                 float growth_factor = 2.0f);
    
    /**
     * @brief 析构函数
     */
    ~MultiLevelMemoryPool();
    
    /**
     * @brief 分配内存
     * @param size 需要的内存大小
     * @return 分配的内存指针
     */
    void* allocate(size_t size);
    
    /**
     * @brief 释放内存
     * @param ptr 要释放的内存指针
     * @param size 内存大小
     */
    void deallocate(void* ptr, size_t size);
    
    /**
     * @brief 获取统计信息
     * @return 统计信息字符串
     */
    std::string get_stats() const;
    
    /**
     * @brief 清空所有内存池
     */
    void clear();
    
private:
    std::vector<std::unique_ptr<MemoryPool>> pools_; ///< 内存池列表
    size_t max_block_size_;                          ///< 最大内存块大小
    float growth_factor_;                            ///< 增长因子
    mutable std::mutex mutex_;                       ///< 互斥锁
    
    /**
     * @brief 找到合适的内存池索引
     * @param size 需要的内存大小
     * @return 内存池索引
     */
    size_t find_pool_index(size_t size) const;
    
    /**
     * @brief 确保内存池存在
     * @param index 内存池索引
     */
    void ensure_pool_exists(size_t index);
};

/**
 * @brief 线程本地内存池
 * 每个线程独立的内存池，减少锁竞争
 */
class ThreadLocalMemoryPool {
public:
    /**
     * @brief 构造函数
     * @param block_size 内存块大小
     * @param initial_blocks 初始内存块数量
     */
    explicit ThreadLocalMemoryPool(size_t block_size, size_t initial_blocks = 10);
    
    /**
     * @brief 析构函数
     */
    ~ThreadLocalMemoryPool();
    
    /**
     * @brief 分配内存
     * @return 分配的内存指针
     */
    void* allocate();
    
    /**
     * @brief 释放内存
     * @param ptr 要释放的内存指针
     */
    void deallocate(void* ptr);
    
    /**
     * @brief 获取当前线程的内存池
     * @return 内存池指针
     */
    MemoryPool* get_current_pool();
    
private:
    size_t block_size_;                              ///< 内存块大小
    size_t initial_blocks_;                          ///< 初始内存块数量
    std::unordered_map<std::thread::id, std::unique_ptr<MemoryPool>> thread_pools_; ///< 线程内存池映射
    mutable std::mutex pools_mutex_;                 ///< 内存池映射互斥锁
    
    /**
     * @brief 创建线程内存池
     * @return 新创建的内存池
     */
    std::unique_ptr<MemoryPool> create_thread_pool();
};

/**
 * @brief 内存池管理器
 * 全局内存池管理
 */
class MemoryPoolManager {
public:
    /**
     * @brief 获取单例实例
     * @return 内存池管理器实例
     */
    static MemoryPoolManager& get();
    
    /**
     * @brief 分配内存
     * @param size 需要的内存大小
     * @return 分配的内存指针
     */
    void* allocate(size_t size);
    
    /**
     * @brief 释放内存
     * @param ptr 要释放的内存指针
     * @param size 内存大小
     */
    void deallocate(void* ptr, size_t size);
    
    /**
     * @brief 获取统计信息
     * @return 统计信息字符串
     */
    std::string get_stats() const;
    
    /**
     * @brief 清空所有内存池
     */
    void clear();
    
private:
    MemoryPoolManager() = default;
    ~MemoryPoolManager() = default;
    
    MultiLevelMemoryPool multi_pool_;                ///< 多级内存池
    ThreadLocalMemoryPool thread_local_pool_;        ///< 线程本地内存池
    mutable std::mutex mutex_;                       ///< 互斥锁
    
    // 禁用拷贝和赋值
    MemoryPoolManager(const MemoryPoolManager&) = delete;
    MemoryPoolManager& operator=(const MemoryPoolManager&) = delete;
};

} // namespace sugarlog 