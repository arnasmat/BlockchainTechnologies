cmake_minimum_required(VERSION 3.31)
project(BlockchainTechnologies)

set(CMAKE_CXX_COMPILER "g++")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
set(CMAKE_CXX_STANDARD 20)

find_package(OpenSSL REQUIRED)

file(GLOB SRC_FILES src/*.cpp)
add_executable(BlockchainTechnologies main.cpp ${SRC_FILES})

target_include_directories(BlockchainTechnologies PRIVATE include)

set_target_properties(BlockchainTechnologies PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/../bin"
)
target_link_libraries(BlockchainTechnologies PRIVATE OpenSSL::SSL OpenSSL::Crypto)
