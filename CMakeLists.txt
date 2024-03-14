cmake_minimum_required(VERSION 3.28)

project(GooE
    VERSION 0.1.0.0
    DESCRIPTION "A Game Engine written in C"
    HOMEPAGE_URL "https://github.com/Gamabunta57/GooE"
    LANGUAGES C)

set(DESTINATION_EXECUTABLE_FOLDER $<1:${PROJECT_SOURCE_DIR}>/output/bin/$<CONFIG>)
set(DESTINATION_LIB_FOLDER $<1:${PROJECT_SOURCE_DIR}>/output/lib/$<CONFIG>)

set(CMAKE_CONFIGURATION_TYPES "Release;Debug")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${DESTINATION_EXECUTABLE_FOLDER})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${DESTINATION_LIB_FOLDER})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${DESTINATION_LIB_FOLDER})

set(C_STANDARD "C90")
set(C_STANDARD_REQUIRED TRUE)
set(SDL3_ROOT_PATH ${PROJECT_SOURCE_DIR}/vendors/build/sdl)
set(SDL3_IMAGE_ROOT_PATH ${PROJECT_SOURCE_DIR}/vendors/build/sdl_image)
set(SDL3_MIXER_ROOT_PATH ${PROJECT_SOURCE_DIR}/vendors/build/sdl_mixer)

configure_file(src/meta.h.in meta.h
    NEWLINE_STYLE UNIX
)

add_executable(gooe src/main.c)
add_subdirectory(src/libs/gooeLog)

target_include_directories(gooe
    PUBLIC ${PROJECT_BINARY_DIR}
    PUBLIC ./src/libs
    PUBLIC vendors/build/sdl/include
    PUBLIC vendors/build/sdl_image/include
    PUBLIC vendors/build/sdl_mixer/include
)


add_library(sdl3 SHARED IMPORTED GLOBAL)
set_property(TARGET sdl3 PROPERTY IMPORTED_IMPLIB_RELEASE "${SDL3_ROOT_PATH}/lib/SDL3.lib")
set_property(TARGET sdl3 PROPERTY IMPORTED_LOCATION_RELEASE "${SDL3_ROOT_PATH}/bin/SDL3.dll")
#set_property(TARGET sdl3 PROPERTY IMPORTED_IMPLIB_DEBUG "${SDL3_ROOT_PATH}/lib/SDL3d.lib")
#set_property(TARGET sdl3 PROPERTY IMPORTED_LOCATION_DEBUG "${SDL3_ROOT_PATH}/bin/SDL3d.dll")
set_property(TARGET sdl3 PROPERTY IMPORTED_IMPLIB_DEBUG "${SDL3_ROOT_PATH}/lib/SDL3.lib")
set_property(TARGET sdl3 PROPERTY IMPORTED_LOCATION_DEBUG "${SDL3_ROOT_PATH}/bin/SDL3.dll")


add_library(sdl3_image SHARED IMPORTED GLOBAL)
set_property(TARGET sdl3_image PROPERTY IMPORTED_IMPLIB_RELEASE "${SDL3_IMAGE_ROOT_PATH}/lib/SDL3_image.lib")
set_property(TARGET sdl3_image PROPERTY IMPORTED_LOCATION_RELEASE "${SDL3_IMAGE_ROOT_PATH}/bin/SDL3_image.dll")
#set_property(TARGET sdl3_image PROPERTY IMPORTED_IMPLIB_DEBUG "${SDL3_IMAGE_ROOT_PATH}/lib/SDL3d_image.lib")
#set_property(TARGET sdl3_image PROPERTY IMPORTED_LOCATION_DEBUG "${SDL3_IMAGE_ROOT_PATH}/bin/SDL3d_image.dll")
set_property(TARGET sdl3_image PROPERTY IMPORTED_IMPLIB_DEBUG "${SDL3_IMAGE_ROOT_PATH}/lib/SDL3_image.lib")
set_property(TARGET sdl3_image PROPERTY IMPORTED_LOCATION_DEBUG "${SDL3_IMAGE_ROOT_PATH}/bin/SDL3_image.dll")


add_library(sdl3_mixer SHARED IMPORTED GLOBAL)
set_property(TARGET sdl3_mixer PROPERTY IMPORTED_IMPLIB_RELEASE "${SDL3_MIXER_ROOT_PATH}/lib/SDL3_mixer.lib")
set_property(TARGET sdl3_mixer PROPERTY IMPORTED_LOCATION_RELEASE "${SDL3_MIXER_ROOT_PATH}/bin/SDL3_mixer.dll")
#set_property(TARGET sdl3_mixer PROPERTY IMPORTED_IMPLIB_DEBUG "${SDL3_MIXER_ROOT_PATH}/lib/SDL3d_mixer.lib")
#set_property(TARGET sdl3_mixer PROPERTY IMPORTED_LOCATION_DEBUG "${SDL3_MIXER_ROOT_PATH}/bin/SDL3d_mixer.dll")
set_property(TARGET sdl3_mixer PROPERTY IMPORTED_IMPLIB_DEBUG "${SDL3_MIXER_ROOT_PATH}/lib/SDL3_mixer.lib")
set_property(TARGET sdl3_mixer PROPERTY IMPORTED_LOCATION_DEBUG "${SDL3_MIXER_ROOT_PATH}/bin/SDL3_mixer.dll")


target_link_libraries(gooe
    gooeLog
    sdl3
    sdl3_image
    sdl3_mixer
)

add_custom_command (TARGET gooe POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    $<TARGET_FILE:sdl3> $<TARGET_FILE_DIR:gooe>/
    $<TARGET_FILE:sdl3_image> $<TARGET_FILE_DIR:gooe>/
    $<TARGET_FILE:sdl3_mixer> $<TARGET_FILE_DIR:gooe>/
)

add_custom_command (TARGET gooe POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different
    ${PROJECT_SOURCE_DIR}/assets $<TARGET_FILE_DIR:gooe>/assets
)