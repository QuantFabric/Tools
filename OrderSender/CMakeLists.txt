cmake_minimum_required(VERSION 3.16)

PROJECT(OrderSender)

set(CMAKE_CXX_FLAGS "-fPIC")

set(CMAKE_INCLUDE_CURRENT_DIR ON)

# 默认输出debug版
SET(CMAKE_BUILD_TYPE "Release")
#SET(CMAKE_BUILD_TYPE "debug")
set(CMAKE_CXX_STANDARD 11)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../Utils/)
# YAML-CPP
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../XAPI/YAML-CPP/0.8.0/include/)
link_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../XAPI/YAML-CPP/0.8.0/lib)

set(SOURCES OrderSender.cpp)

add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME} yaml-cpp)
