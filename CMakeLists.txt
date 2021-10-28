cmake_minimum_required(VERSION 3.8)

project(MidnightEngine VERSION 0.01)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

file(GLOB_RECURSE SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/Source/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/Source/*.c)
file(GLOB_RECURSE HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/Headers/*.h)
file(GLOB_RECURSE RESOURCES ${CMAKE_CURRENT_SOURCE_DIR}/Resources/*)
file(GLOB_RECURSE LIB_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/Libraries/include/*.h)

if(WIN32)
set(OS "Windows")
endif(WIN32)

if(APPLE)
set(OS "macOS")
endif()

set(LIBS_DIR ${CMAKE_SOURCE_DIR}/Libraries/bin/${OS})

add_executable(MidnightEngine ${SOURCES} ${HEADERS} ${RESOURCES})

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

function(makeCategory FOLDER LIST)
    foreach(FILE ${LIST})
        get_filename_component(PARENT_DIR "${FILE}" DIRECTORY)
        string(REPLACE "${CMAKE_CURRENT_SOURCE_DIR}/${FOLDER}" "" GROUP "${PARENT_DIR}")
        string(REPLACE "/" "\\" GROUP "${GROUP}")
        set(GROUP "${FOLDER}${GROUP}")
        source_group("${GROUP}" FILES "${FILE}")
    endforeach()
endfunction()

makeCategory(Headers "${HEADERS}")
makeCategory(Source "${SOURCES}")
makeCategory(Resources "${RESOURCES}")

target_include_directories(MidnightEngine PUBLIC ${CMAKE_SOURCE_DIR}/Headers)
target_include_directories(MidnightEngine PUBLIC ${CMAKE_SOURCE_DIR}/Libraries/include)

if(APPLE)

find_library(FOUNDATION Foundation)
find_library(COCOA Cocoa)
find_library(IOKIT IoKit)
target_link_libraries(MidnightEngine ${LIBS_DIR}/GLFW/arm64/libglfw3.a)
target_link_libraries(MidnightEngine ${LIBS_DIR}/GLFW/arm64/libglfw.3.dylib)
target_link_libraries(MidnightEngine ${FOUNDATION} ${COCOA} ${IOKIT})
add_custom_command(TARGET MidnightEngine
                   POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/Resources $<TARGET_FILE_DIR:MidnightEngine>/Resources)

endif()

if(WIN32)

target_link_libraries(MidnightEngine ${LIBS_DIR}/GLFW/x64/glfw3.lib)
add_custom_command(TARGET MidnightEngine
                   POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/Resources ${CMAKE_BINARY_DIR}/Resources)

endif()

