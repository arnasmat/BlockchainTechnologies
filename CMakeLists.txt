cmake_minimum_required(VERSION 3.31)
project(BlockchainTechnologies)

set(CMAKE_CXX_COMPILER "g++")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
set(CMAKE_CXX_STANDARD 20)


file(GLOB_RECURSE SRC_FILES src/*.cpp)
add_executable(hashCli hashCli.cpp ${SRC_FILES})
add_executable(blockchain main.cpp ${SRC_FILES})

target_include_directories(hashCli PRIVATE include)
target_include_directories(blockchain PRIVATE include)

set_target_properties(hashCli PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/../bin"
)

set_target_properties(blockchain PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/../bin"
)

#find_package(OpenSSL REQUIRED)
#target_link_libraries(BlockchainTechnologies PRIVATE OpenSSL::SSL OpenSSL::Crypto)
