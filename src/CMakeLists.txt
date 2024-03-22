add_executable(gooe main.c)

set_source_files_properties(${SOURCES} PROPERTIES LANGUAGE C)
set_target_properties(gooe PROPERTIES LINKER_LANGUAGE C)

add_subdirectory(libs)

if (CMAKE_BUILD_TYPE STREQUAL "Release")
    add_compile_definitions(GOOE_LOG_NONE)
endif()

configure_file(meta.h.in meta.h
    NEWLINE_STYLE UNIX
)

target_include_directories(gooe
    PUBLIC ./libs
    PUBLIC ${GOOE_VENDORS_BUILDS}
    PUBLIC ${PROJECT_BINARY_DIR}/src
)

target_link_libraries(gooe gooeLog)

set(GOOE_VENDORS_BUILDS ${PROJECT_SOURCE_DIR}/vendors/builds)

set(CMAKE_FIND_PACKAGE_REDIRECTS_DIR ${GOOE_VENDORS_BUILDS})

find_package(SDL3 REQUIRED)
find_package(SDL3_image REQUIRED)
find_package(SDL3_mixer REQUIRED)

target_link_libraries(gooe 
    SDL3::SDL3
    SDL3_image::SDL3_image
    SDL3_mixer::SDL3_mixer
)

add_custom_command (TARGET gooe POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    $<TARGET_FILE:SDL3::SDL3> $<TARGET_FILE_DIR:gooe>/
    $<TARGET_FILE:SDL3_image::SDL3_image> $<TARGET_FILE_DIR:gooe>/
    $<TARGET_FILE:SDL3_mixer::SDL3_mixer> $<TARGET_FILE_DIR:gooe>/
    $<TARGET_FILE:gooeLog> $<TARGET_FILE_DIR:gooe>/
)

add_custom_command (TARGET gooe POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different
    ${PROJECT_SOURCE_DIR}/assets $<TARGET_FILE_DIR:gooe>/assets
)