file(GLOB_RECURSE SOURCE_FILES
    "${CMAKE_CURRENT_LIST_DIR}/*.c"
     "${CMAKE_CURRENT_LIST_DIR}/*.cpp"
     "${CMAKE_CURRENT_LIST_DIR}/*.h"
)

add_executable(about-c-plus-plus
    ${SOURCE_FILES}
)

source_group(TREE "${CMAKE_CURRENT_LIST_DIR}" FILES ${SOURCE_FILES})
