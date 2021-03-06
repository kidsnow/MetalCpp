set(TargetName renderer)

add_library(
    ${TargetName}
    Renderer.cpp
    RenderTarget.cpp
    ViewDelegate.cpp
)

find_library(Cocoa_lib Cocoa)
find_library(Metal_lib Metal)
find_library(Foundation_lib Foundation)
find_library(CoreGraphics_lib CoreGraphics)
find_library(MetalKit_lib MetalKit)

set(
    Framework
    ${Cocoa_lib}
    ${Metal_lib}
    ${Foundation_lib}
    ${CoreGraphics_lib}
    ${MetalKit_lib}
)

target_link_libraries(
    ${TargetName} PUBLIC
    ${Framework}
)

target_include_directories(
    ${TargetName} PUBLIC
    ${PROJECT_SOURCE_DIR}/src
    ${PROJECT_SOURCE_DIR}/lib/metal-cpp-extensions
    ${PROJECT_SOURCE_DIR}/lib/metal-cpp
)