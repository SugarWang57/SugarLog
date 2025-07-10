/**
 * @file thread_safe_queue.hpp
 * @brief 线程安全队列
 */

#pragma once

#include <queue>
#include <mutex>
#include <condition_variable>
#include <memory>
#include <atomic>
#include <chrono>

namespace sugarlog {

/**
 * @brief 线程安全队列基类
 * @tparam T 队列元素类型
 */
template<typename T>
class ThreadSafeQueue {
public:
    using value_type = T;
    using size_type = typename std::queue<T>::size_type;
    
    /**
     * @brief 构造函数
     * @param max_size 最大队列大小，0表示无限制
     */
    explicit ThreadSafeQueue(size_t max_size = 0) 
        : max_size_(max_size), shutdown_(false) {}
    
    /**
     * @brief 析构函数
     */
    virtual ~ThreadSafeQueue() = default;
    
    /**
     * @brief 推入元素（阻塞）
     * @param item 要推入的元素
     * @return 是否成功推入
     */
    bool push(const T& item) {
        std::unique_lock<std::mutex> lock(mutex_);
        if (shutdown_) return false;
        
        // 等待队列有空间
        if (max_size_ > 0) {
            not_full_.wait(lock, [this] { 
                return queue_.size() < max_size_ || shutdown_; 
            });
            if (shutdown_) return false;
        }
        
        queue_.push(item);
        not_empty_.notify_one();
        return true;
    }
    
    /**
     * @brief 推入元素（移动语义）
     * @param item 要推入的元素
     * @return 是否成功推入
     */
    bool push(T&& item) {
        std::unique_lock<std::mutex> lock(mutex_);
        if (shutdown_) return false;
        
        // 等待队列有空间
        if (max_size_ > 0) {
            not_full_.wait(lock, [this] { 
                return queue_.size() < max_size_ || shutdown_; 
            });
            if (shutdown_) return false;
        }
        
        queue_.push(std::move(item));
        not_empty_.notify_one();
        return true;
    }
    
    /**
     * @brief 尝试推入元素（非阻塞）
     * @param item 要推入的元素
     * @return 是否成功推入
     */
    bool try_push(const T& item) {
        std::unique_lock<std::mutex> lock(mutex_);
        if (shutdown_) return false;
        
        if (max_size_ > 0 && queue_.size() >= max_size_) {
            return false;
        }
        
        queue_.push(item);
        not_empty_.notify_one();
        return true;
    }
    
    /**
     * @brief 尝试推入元素（非阻塞，移动语义）
     * @param item 要推入的元素
     * @return 是否成功推入
     */
    bool try_push(T&& item) {
        std::unique_lock<std::mutex> lock(mutex_);
        if (shutdown_) return false;
        
        if (max_size_ > 0 && queue_.size() >= max_size_) {
            return false;
        }
        
        queue_.push(std::move(item));
        not_empty_.notify_one();
        return true;
    }
    
    /**
     * @brief 弹出元素（阻塞）
     * @param item 接收弹出元素的引用
     * @return 是否成功弹出
     */
    bool pop(T& item) {
        std::unique_lock<std::mutex> lock(mutex_);
        not_empty_.wait(lock, [this] { 
            return !queue_.empty() || shutdown_; 
        });
        
        if (shutdown_ && queue_.empty()) {
            return false;
        }
        
        item = std::move(queue_.front());
        queue_.pop();
        not_full_.notify_one();
        return true;
    }
    
    /**
     * @brief 尝试弹出元素（非阻塞）
     * @param item 接收弹出元素的引用
     * @return 是否成功弹出
     */
    bool try_pop(T& item) {
        std::unique_lock<std::mutex> lock(mutex_);
        if (queue_.empty()) {
            return false;
        }
        
        item = std::move(queue_.front());
        queue_.pop();
        not_full_.notify_one();
        return true;
    }
    
    /**
     * @brief 带超时的弹出元素
     * @param item 接收弹出元素的引用
     * @param timeout 超时时间
     * @return 是否成功弹出
     */
    template<typename Rep, typename Period>
    bool pop(T& item, const std::chrono::duration<Rep, Period>& timeout) {
        std::unique_lock<std::mutex> lock(mutex_);
        if (!not_empty_.wait_for(lock, timeout, [this] { 
            return !queue_.empty() || shutdown_; 
        })) {
            return false;
        }
        
        if (shutdown_ && queue_.empty()) {
            return false;
        }
        
        item = std::move(queue_.front());
        queue_.pop();
        not_full_.notify_one();
        return true;
    }
    
    /**
     * @brief 获取队列大小
     * @return 队列大小
     */
    size_type size() const {
        std::unique_lock<std::mutex> lock(mutex_);
        return queue_.size();
    }
    
    /**
     * @brief 检查队列是否为空
     * @return 是否为空
     */
    bool empty() const {
        std::unique_lock<std::mutex> lock(mutex_);
        return queue_.empty();
    }
    
    /**
     * @brief 检查队列是否已满
     * @return 是否已满
     */
    bool full() const {
        if (max_size_ == 0) return false;
        std::unique_lock<std::mutex> lock(mutex_);
        return queue_.size() >= max_size_;
    }
    
    /**
     * @brief 清空队列
     */
    void clear() {
        std::unique_lock<std::mutex> lock(mutex_);
        while (!queue_.empty()) {
            queue_.pop();
        }
        not_full_.notify_all();
    }
    
    /**
     * @brief 关闭队列
     */
    void shutdown() {
        std::unique_lock<std::mutex> lock(mutex_);
        shutdown_ = true;
        not_empty_.notify_all();
        not_full_.notify_all();
    }
    
    /**
     * @brief 检查队列是否已关闭
     * @return 是否已关闭
     */
    bool is_shutdown() const {
        std::unique_lock<std::mutex> lock(mutex_);
        return shutdown_;
    }
    
    /**
     * @brief 获取最大队列大小
     * @return 最大队列大小
     */
    size_t get_max_size() const { return max_size_; }
    
    /**
     * @brief 设置最大队列大小
     * @param max_size 最大队列大小
     */
    void set_max_size(size_t max_size) {
        std::unique_lock<std::mutex> lock(mutex_);
        max_size_ = max_size;
        not_full_.notify_all();
    }
    
protected:
    std::queue<T> queue_;                    ///< 底层队列
    mutable std::mutex mutex_;               ///< 互斥锁
    std::condition_variable not_empty_;      ///< 非空条件变量
    std::condition_variable not_full_;       ///< 非满条件变量
    size_t max_size_;                        ///< 最大队列大小
    std::atomic<bool> shutdown_;             ///< 关闭标志
};

/**
 * @brief 批量处理队列
 * 支持批量获取和处理的队列
 */
template<typename T>
class BatchQueue : public ThreadSafeQueue<T> {
public:
    using Batch = std::vector<T>;
    
    /**
     * @brief 构造函数
     * @param max_size 最大队列大小
     * @param batch_size 批量大小
     */
    explicit BatchQueue(size_t max_size = 0, size_t batch_size = 100)
        : ThreadSafeQueue<T>(max_size), batch_size_(batch_size) {}
    
    /**
     * @brief 批量弹出元素（阻塞）
     * @param batch 接收批量元素的容器
     * @param max_items 最大获取数量
     * @return 实际获取的数量
     */
    size_t pop_batch(Batch& batch, size_t max_items = 0) {
        if (max_items == 0) max_items = batch_size_;
        
        std::unique_lock<std::mutex> lock(this->mutex_);
        this->not_empty_.wait(lock, [this] { 
            return !this->queue_.empty() || this->shutdown_; 
        });
        
        if (this->shutdown_ && this->queue_.empty()) {
            return 0;
        }
        
        size_t count = 0;
        batch.clear();
        batch.reserve(max_items);
        
        while (!this->queue_.empty() && count < max_items) {
            batch.push_back(std::move(this->queue_.front()));
            this->queue_.pop();
            count++;
        }
        
        this->not_full_.notify_all();
        return count;
    }
    
    /**
     * @brief 批量弹出元素（非阻塞）
     * @param batch 接收批量元素的容器
     * @param max_items 最大获取数量
     * @return 实际获取的数量
     */
    size_t try_pop_batch(Batch& batch, size_t max_items = 0) {
        if (max_items == 0) max_items = batch_size_;
        
        std::unique_lock<std::mutex> lock(this->mutex_);
        if (this->queue_.empty()) {
            return 0;
        }
        
        size_t count = 0;
        batch.clear();
        batch.reserve(max_items);
        
        while (!this->queue_.empty() && count < max_items) {
            batch.push_back(std::move(this->queue_.front()));
            this->queue_.pop();
            count++;
        }
        
        this->not_full_.notify_all();
        return count;
    }
    
    /**
     * @brief 带超时的批量弹出元素
     * @param batch 接收批量元素的容器
     * @param timeout 超时时间
     * @param max_items 最大获取数量
     * @return 实际获取的数量
     */
    template<typename Rep, typename Period>
    size_t pop_batch(Batch& batch, 
                     const std::chrono::duration<Rep, Period>& timeout,
                     size_t max_items = 0) {
        if (max_items == 0) max_items = batch_size_;
        
        std::unique_lock<std::mutex> lock(this->mutex_);
        if (!this->not_empty_.wait_for(lock, timeout, [this] { 
            return !this->queue_.empty() || this->shutdown_; 
        })) {
            return 0;
        }
        
        if (this->shutdown_ && this->queue_.empty()) {
            return 0;
        }
        
        size_t count = 0;
        batch.clear();
        batch.reserve(max_items);
        
        while (!this->queue_.empty() && count < max_items) {
            batch.push_back(std::move(this->queue_.front()));
            this->queue_.pop();
            count++;
        }
        
        this->not_full_.notify_all();
        return count;
    }
    
    /**
     * @brief 设置批量大小
     * @param batch_size 批量大小
     */
    void set_batch_size(size_t batch_size) { batch_size_ = batch_size; }
    
    /**
     * @brief 获取批量大小
     * @return 批量大小
     */
    size_t get_batch_size() const { return batch_size_; }

    size_t capacity() const { return this->get_max_size(); }
    
private:
    size_t batch_size_;                     ///< 批量大小
};

/**
 * @brief 优先级队列
 * 支持优先级的线程安全队列
 */
template<typename T, typename Compare = std::less<T>>
class PriorityQueue : public ThreadSafeQueue<T> {
public:
    /**
     * @brief 构造函数
     * @param max_size 最大队列大小
     * @param comp 比较函数
     */
    explicit PriorityQueue(size_t max_size = 0, Compare comp = Compare())
        : ThreadSafeQueue<T>(max_size), comp_(comp) {}
    
    /**
     * @brief 推入元素（阻塞）
     * @param item 要推入的元素
     * @return 是否成功推入
     */
    bool push(const T& item) {
        std::unique_lock<std::mutex> lock(this->mutex_);
        if (this->shutdown_) return false;
        
        // 等待队列有空间
        if (this->max_size_ > 0) {
            this->not_full_.wait(lock, [this] { 
                return priority_queue_.size() < this->max_size_ || this->shutdown_; 
            });
            if (this->shutdown_) return false;
        }
        
        priority_queue_.push(item);
        this->not_empty_.notify_one();
        return true;
    }
    
    /**
     * @brief 推入元素（移动语义）
     * @param item 要推入的元素
     * @return 是否成功推入
     */
    bool push(T&& item) {
        std::unique_lock<std::mutex> lock(this->mutex_);
        if (this->shutdown_) return false;
        
        // 等待队列有空间
        if (this->max_size_ > 0) {
            this->not_full_.wait(lock, [this] { 
                return priority_queue_.size() < this->max_size_ || this->shutdown_; 
            });
            if (this->shutdown_) return false;
        }
        
        priority_queue_.push(std::move(item));
        this->not_empty_.notify_one();
        return true;
    }
    
    /**
     * @brief 弹出元素（阻塞）
     * @param item 接收弹出元素的引用
     * @return 是否成功弹出
     */
    bool pop(T& item) {
        std::unique_lock<std::mutex> lock(this->mutex_);
        this->not_empty_.wait(lock, [this] { 
            return !priority_queue_.empty() || this->shutdown_; 
        });
        
        if (this->shutdown_ && priority_queue_.empty()) {
            return false;
        }
        
        item = std::move(const_cast<T&>(priority_queue_.top()));
        priority_queue_.pop();
        this->not_full_.notify_one();
        return true;
    }
    
    /**
     * @brief 尝试弹出元素（非阻塞）
     * @param item 接收弹出元素的引用
     * @return 是否成功弹出
     */
    bool try_pop(T& item) {
        std::unique_lock<std::mutex> lock(this->mutex_);
        if (priority_queue_.empty()) {
            return false;
        }
        
        item = std::move(const_cast<T&>(priority_queue_.top()));
        priority_queue_.pop();
        this->not_full_.notify_one();
        return true;
    }
    
    /**
     * @brief 获取队列大小
     * @return 队列大小
     */
    typename std::priority_queue<T, std::vector<T>, Compare>::size_type size() const {
        std::unique_lock<std::mutex> lock(this->mutex_);
        return priority_queue_.size();
    }
    
    /**
     * @brief 检查队列是否为空
     * @return 是否为空
     */
    bool empty() const {
        std::unique_lock<std::mutex> lock(this->mutex_);
        return priority_queue_.empty();
    }
    
    /**
     * @brief 清空队列
     */
    void clear() {
        std::unique_lock<std::mutex> lock(this->mutex_);
        while (!priority_queue_.empty()) {
            priority_queue_.pop();
        }
        this->not_full_.notify_all();
    }
    
private:
    std::priority_queue<T, std::vector<T>, Compare> priority_queue_; ///< 底层优先级队列
    Compare comp_;                                                    ///< 比较函数
};

} // namespace sugarlog 