cmake_minimum_required(VERSION 3.10)
project(libp2p)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup(TARGETS)

set(CMAKE_CXX_STANDARD 17)

find_package(Protobuf REQUIRED)
find_package(gtest REQUIRED)
find_package(multiformats REQUIRED)
find_package(cryptopp REQUIRED)
find_package(asio REQUIRED)

file(GLOB PROTOS proto/*.proto)
protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS ${PROTOS})

add_library(${PROJECT_NAME} ${PROTO_SRCS} ${PROTO_HDRS}
    src/general/peer-id.cpp)

target_link_libraries(${PROJECT_NAME} PUBLIC CONAN_PKG::protobuf
    asio::asio
    multiformats::multiformats
    cryptopp::cryptopp)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_BINARY_DIR}
    PUBLIC include)

# Tests

add_executable(libp2pTests test/src/libp2p-tests.cpp)
target_link_libraries(libp2pTests PRIVATE ${PROJECT_NAME} gtest::gtest)

add_executable(initiator test/src/initiator.cpp)
target_link_libraries(initiator PRIVATE ${PROJECT_NAME})

add_executable(responder test/src/responder.cpp)
target_link_libraries(responder PRIVATE ${PROJECT_NAME})
