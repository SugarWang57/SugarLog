# SugarLog

高性能、现代化的C++日志库，支持多线程、异步、格式化、性能监控等特性。

## 特性
- 线程安全、异步日志
- 多种日志级别和格式化
- 控制台/文件/自定义输出
- 性能监控与统计
- 易于集成和扩展

## 构建与导入

### 1. 构建静态库
```bash
mkdir build && cd build
cmake ..
make
```
生成 `libsugarlog.a` 静态库和头文件。

### 2. 静态库使用方法

#### 方法一：CMake 集成（推荐）
```cmake
# 方式1：直接导入静态库
include_directories(/path/to/sugarlog/include)
add_library(sugarlog STATIC IMPORTED)
set_target_properties(sugarlog PROPERTIES 
    IMPORTED_LOCATION /path/to/sugarlog/build/libsugarlog.a
)
target_link_libraries(your_target sugarlog pthread)

# 方式2：add_subdirectory 方式
add_subdirectory(/path/to/sugarlog)
target_link_libraries(your_target sugarlog)
```

#### 方法二：Makefile 集成
```makefile
# 设置路径
SUGARLOG_DIR = /path/to/sugarlog
SUGARLOG_INCLUDE = $(SUGARLOG_DIR)/include
SUGARLOG_LIB = $(SUGARLOG_DIR)/build/libsugarlog.a

# 编译选项
CXXFLAGS += -I$(SUGARLOG_INCLUDE)
LDFLAGS += $(SUGARLOG_LIB) -lpthread

# 目标
your_target: your_source.cpp
	$(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS)
```

#### 方法三：g++ 直接编译
```bash
# 基本编译
g++ -std=c++20 -I/path/to/sugarlog/include \
    your_source.cpp \
    /path/to/sugarlog/build/libsugarlog.a \
    -lpthread -o your_program

# 带优化编译
g++ -std=c++20 -O3 -I/path/to/sugarlog/include \
    your_source.cpp \
    /path/to/sugarlog/build/libsugarlog.a \
    -lpthread -o your_program
```

#### 方法四：Visual Studio (Windows)
```cpp
// 项目设置中添加：
// 1. 包含目录: /path/to/sugarlog/include
// 2. 库目录: /path/to/sugarlog/build
// 3. 链接库: sugarlog.lib (或 libsugarlog.a)
// 4. 系统库: pthread (Windows下可能不需要)
```

### 3. 完整集成示例

#### CMake 项目示例
```cmake
cmake_minimum_required(VERSION 3.16)
project(MyProject)

set(CMAKE_CXX_STANDARD 20)

# 导入 SugarLog
include_directories(/path/to/sugarlog/include)
add_library(sugarlog STATIC IMPORTED)
set_target_properties(sugarlog PROPERTIES 
    IMPORTED_LOCATION /path/to/sugarlog/build/libsugarlog.a
)

# 你的项目
add_executable(my_app main.cpp)
target_link_libraries(my_app sugarlog pthread)
```

#### 源文件示例
```cpp
#include "sugarlog.hpp"
#include "console_sink.hpp"

int main() {
    using namespace sugarlog;
    
    // 初始化日志系统
    LogManager::get().initialize();
    LogManager::get().add_sink(std::make_shared<ConsoleSink>());
    
    // 使用日志
    LOG_INFO("Hello, SugarLog!");
    LOG_ERROR("Something went wrong!");
    
    return 0;
}
```

### 4. 依赖说明
- **C++20**: 需要支持 C++20 的编译器
- **pthread**: 需要链接 pthread 库（Linux/macOS）
- **头文件**: 需要包含 `include/` 目录

### 5. 常见问题

#### 链接错误
```bash
# 如果遇到链接错误，确保：
# 1. 静态库路径正确
# 2. 已链接 pthread
# 3. 编译器支持 C++20
```

#### 编译错误
```bash
# 如果遇到编译错误，检查：
# 1. 头文件路径是否正确
# 2. 是否使用了正确的 C++ 标准
# 3. 是否包含了必要的头文件
```

## 快速使用示例
```cpp
#include "sugarlog.hpp"

int main() {
    using namespace sugarlog;
    LogManager::get().initialize();
    LogManager::get().add_sink(std::make_shared<ConsoleSink>());
    LOG_INFO("Hello, SugarLog!");
    LOG_ERROR("Something went wrong!");
    return 0;
}
```

## 主要API手册

### 日志管理器 LogManager
- `static LogManager& get()` 获取全局单例
- `void initialize(const LogConfig& config = LogConfig{})` 初始化
- `void add_sink(std::shared_ptr<LogSink> sink)` 添加输出目标
- `void set_level(LogLevel level)` 设置日志级别
- `void log(LogLevel, const std::string&, ...)` 记录日志
- `void shutdown()` 关闭日志系统

### 日志输出目标 LogSink 及其子类
- `ConsoleSink` 控制台输出
- `FileSink` 文件输出
- `CompositeLogSink` 复合输出

### 日志宏
- `LOG_TRACE(msg)`
- `LOG_DEBUG(msg)`
- `LOG_INFO(msg)`
- `LOG_WARN(msg)`
- `LOG_ERROR(msg)`
- `LOG_FATAL(msg)`

### 性能监控 PerformanceMonitor
- `PerformanceMonitor::get()` 获取全局监控器
- `void start(const std::string& name)` 开始监控
- `void end(const std::string& name)` 结束监控
- `PerformanceStats get_stats(const std::string& name)` 获取统计

### 其他
- `LogLevel` 日志级别枚举
- `LogFormatter` 日志格式化器
- `LogMessage` 日志消息结构体

---
详细API和高级用法请参考源码注释和 `include/` 头文件。
