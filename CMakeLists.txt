cmake_minimum_required(VERSION 3.14)
project(SmartMemoryVector VERSION 1.0.0)

add_library(smvector INTERFACE)

target_include_directories(smvector INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

target_compile_features(smvector INTERFACE cxx_std_17)

if(SMVECTOR_DEBUG)
    add_executable(test test.cpp)
    target_link_libraries(test PRIVATE smvector)
endif()