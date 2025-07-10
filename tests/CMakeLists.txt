# 测试配置
set(TEST_SOURCES
    test_log_level.cpp
)

# 创建测试可执行文件
add_executable(sugarlog_tests ${TEST_SOURCES})
target_link_libraries(sugarlog_tests sugarlog)

# 添加测试
add_test(NAME LogLevelTest COMMAND sugarlog_tests)

# 设置测试属性
set_tests_properties(LogLevelTest PROPERTIES TIMEOUT 30) 