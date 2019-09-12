cmake_minimum_required(VERSION 3.10)
project(libp2p)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup(TARGETS)

find_package(Protobuf REQUIRED)

file(GLOB PROTOS proto/*.proto)
protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS ${PROTOS})
message(STATUS ${PROTO_HDRS})

add_library(${PROJECT_NAME} ${PROTO_SRCS} ${PROTO_HDRS}
    src/general/peer-id.cpp)
target_link_libraries(${PROJECT_NAME} PUBLIC CONAN_PKG::protobuf)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_BINARY_DIR}
    PUBLIC include)
