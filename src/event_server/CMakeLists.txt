file(GLOB_RECURSE project_headers 
            "*.h"
            "*.hpp")

file(GLOB_RECURSE project_sources
            "*.cc"
            "*.cpp")

set(SRC_LISTS ${project_headers} ${project_sources})
set(OUTPUT_NAME "event_server")

source_group("source" FILES ${SRC_LISTS})

include_directories(.)

add_executable(${OUTPUT_NAME} ${SRC_LISTS})

target_link_libraries(${OUTPUT_NAME} event)
                                                
