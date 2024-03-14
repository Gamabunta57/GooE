file(GLOB SOURCES CONFIGURE_DEPENDS "*.h")

add_library(gooeLog SHARED ${SOURCES})
set_source_files_properties(${SOURCES} PROPERTIES LANGUAGE C)
set_target_properties(gooeLog PROPERTIES LINKER_LANGUAGE C)

add_compile_definitions(GOOE_LOG_IMPLEMENTATION)
