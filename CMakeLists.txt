cmake_minimum_required(VERSION 3.10)
project(libp2p)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup(TARGETS)

find_package(Protobuf REQUIRED)
find_package(gtest REQUIRED)
find_package(multiformats REQUIRED)

file(GLOB PROTOS proto/*.proto)
protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS ${PROTOS})

add_library(${PROJECT_NAME} ${PROTO_SRCS} ${PROTO_HDRS}
    src/general/peer-id.cpp
    src/transport/tcp.cpp)

target_link_libraries(${PROJECT_NAME} PUBLIC CONAN_PKG::protobuf
    multiformats::multiformats)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_BINARY_DIR}
    PUBLIC include)
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)

# Tests
add_executable(libp2pTest test/src/tcp.cpp)
target_link_libraries(libp2pTest PRIVATE ${PROJECT_NAME} gtest::gtest)
