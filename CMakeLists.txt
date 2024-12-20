cmake_minimum_required(VERSION 3.26)
project(pong-sfml)

find_package(SFML COMPONENTS system window graphics REQUIRED)

add_executable(${CMAKE_PROJECT_NAME} main.cpp)

target_compile_options(${CMAKE_PROJECT_NAME} PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:/W3 /permissive- /TP>
    $<$<CXX_COMPILER_ID:GNU>:-Wall -Wextra>)

target_compile_definitions(${CMAKE_PROJECT_NAME} PRIVATE $<$<CONFIG:Debug>:DEBUG>)

target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE sfml-graphics)

add_custom_command(TARGET ${CMAKE_PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CMAKE_SOURCE_DIR}/lcd.14.otf" $<TARGET_FILE_DIR:${CMAKE_PROJECT_NAME}>
    COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different "${CMAKE_SOURCE_DIR}/images" $<TARGET_FILE_DIR:${CMAKE_PROJECT_NAME}>/images
)
