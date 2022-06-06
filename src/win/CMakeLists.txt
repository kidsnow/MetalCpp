set(TargetName window)

add_library(
    ${TargetName}
    Window.cpp
    AppDelegate.cpp
)

find_library(CoreGraphics_lib CoreGraphics)

set(
    Framework
    ${CoreGraphics_lib}
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