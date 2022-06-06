set(TargetName App)

add_executable(${TargetName} main.cpp)

target_link_libraries(
    ${TargetName} PUBLIC
    window
    renderer
)

target_include_directories(
    ${TargetName} PUBLIC
    ${PROJECT_SOURCE_DIR}/src
    ${PROJECT_SOURCE_DIR}/lib/metal-cpp-extensions
    ${PROJECT_SOURCE_DIR}/lib/metal-cpp
)