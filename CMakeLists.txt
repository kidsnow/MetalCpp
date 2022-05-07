cmake_minimum_required(VERSION 3.16.2 FATAL_ERROR)

project(MetalCpp LANGUAGES CXX)
set(TargetName MetalCpp)

# 예제에서 사용하는 애플 프레임워크
find_library(Cocoa_lib Cocoa)
find_library(Metal_lib Metal)
find_library(Foundation_lib Foundation)
find_library(CoreGraphics_lib CoreGraphics)
find_library(MetalKit_lib MetalKit)
set(FRAMEWORK ${Cocoa_lib} ${Metal_lib} ${Foundation_lib} ${CoreGraphics_lib} ${MetalKit_lib})

add_executable(${TargetName})
target_sources(${TargetName} PRIVATE src/main.cpp)

set_target_properties(${TargetName} PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)

target_include_directories( ${TargetName} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/metal-cpp-extensions
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/metal-cpp
)

target_link_libraries(${TargetName} ${FRAMEWORK})
