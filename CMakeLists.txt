cmake_minimum_required(VERSION 3.10)
project(libp2p)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup(TARGETS)

find_package(Protobuf REQUIRED)

protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS proto/peer-id.proto)

add_library(${PROJECT_NAME} src/peer-id.cpp)
