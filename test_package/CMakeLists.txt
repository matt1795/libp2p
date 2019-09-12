cmake_minimum_required(VERSION 2.8.12)
project(Libp2pTest CXX)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

add_executable(${PROJECT_NAME} src/peer-id.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE ${CONAN_LIBS})
