#!/bin/bash

# SugarLog 系统安装脚本
echo "正在安装 SugarLog 到系统目录..."

# 创建必要的目录
sudo mkdir -p /usr/local/include/sugarlog
sudo mkdir -p /usr/local/lib
sudo mkdir -p /usr/local/lib/pkgconfig

# 复制头文件
echo "复制头文件..."
sudo cp include/*.hpp /usr/local/include/sugarlog/

# 复制库文件
echo "复制库文件..."
sudo cp build/libsugarlog.a /usr/local/lib/

# 复制 pkg-config 文件
echo "安装 pkg-config 配置..."
sudo cp sugarlog.pc /usr/local/lib/pkgconfig/

# 更新动态链接库缓存
echo "更新动态链接库缓存..."
sudo ldconfig

echo "SugarLog 安装完成！"
echo ""
echo "使用方法："
echo "1. 在代码中直接包含：#include <sugarlog.hpp>"
echo "2. 编译时链接：g++ -o myapp myapp.cpp -lsugarlog"
echo "3. 使用 pkg-config：g++ -o myapp myapp.cpp \$(pkg-config --cflags --libs sugarlog)" 