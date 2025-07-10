cmake_minimum_required(VERSION 3.16)
project(SugarLog VERSION 1.0.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Compiler flags
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -O3")
    set(CMAKE_CXX_FLAGS_DEBUG "-g -O0 -DDEBUG")
    set(CMAKE_CXX_FLAGS_RELEASE "-DNDEBUG -O3")
endif()

# Find required packages
find_package(Threads REQUIRED)

# Include directories
include_directories(include)

# Source files - 添加一些必要的源文件用于静态库
set(SOURCES
    src/sugarlog.cpp
)

# Header files
set(HEADERS
    include/logger.hpp
    include/log_level.hpp
    include/log_message.hpp
    include/log_formatter.hpp
    include/log_sink.hpp
    include/file_sink.hpp
    include/console_sink.hpp
    include/async_logger.hpp
    include/memory_pool.hpp
    include/thread_safe_queue.hpp
    include/log_manager.hpp
    include/performance_monitor.hpp
    include/sugarlog.hpp
)

# Create static library
add_library(sugarlog STATIC ${SOURCES})
target_include_directories(sugarlog PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>)
target_link_libraries(sugarlog PUBLIC Threads::Threads)

# Set library properties
set_target_properties(sugarlog PROPERTIES
    PUBLIC_HEADER "${HEADERS}"
    OUTPUT_NAME "sugarlog"
    VERSION ${PROJECT_VERSION}
    SOVERSION ${PROJECT_VERSION_MAJOR}
)

# Create example executable
add_executable(example examples/basic_usage.cpp)
target_link_libraries(example sugarlog)

# Create performance test
add_executable(performance_test tests/performance_test.cpp)
target_link_libraries(performance_test sugarlog)

# Enable testing
enable_testing()
add_subdirectory(tests)

# Install rules
install(TARGETS sugarlog
    EXPORT sugarlogTargets
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
    PUBLIC_HEADER DESTINATION include
)

install(DIRECTORY include/ DESTINATION include) 