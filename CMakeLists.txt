cmake_minimum_required(VERSION 3.8)

cmake_policy(SET CMP0091 NEW)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_Swift_LANGUAGE_VERSION, 5.0)

file(GLOB_RECURSE ENGINE_RESOURCES ${CMAKE_SOURCE_DIR}/Resources/*)
file(GLOB_RECURSE MODULES ${CMAKE_SOURCE_DIR}/Source/*.Module)
set(MAIN_FILE ${CMAKE_SOURCE_DIR}/Source/MidnightEngine.cpp)
project(MidnightEngine VERSION 0.0.1 LANGUAGES C CXX Swift)

include(${CMAKE_SOURCE_DIR}/Functions.cmake)

foreach(MODULE ${MODULES})

    include(${MODULE})

endforeach()

add_executable(MidnightEngine ${MAIN_FILE} ${ENGINE_SOURCES} ${ENGINE_HEADERS} ${THIRD_PARTY_SOURCES} ${THIRD_PARTY_HEADERS} ${ENGINE_RESOURCES})

filterEngineSources("${MAIN_FILE}")
filterEngineSources("${ENGINE_SOURCES}")
filterEngineSources("${ENGINE_HEADERS}")
filterEngineSources("${ENGINE_RESOURCES}")
filterThirdPartySources("${THIRD_PARTY_SOURCES}")
filterThirdPartySources("${THIRD_PARTY_HEADERS}")

target_include_directories(MidnightEngine PUBLIC "${CMAKE_SOURCE_DIR}/Source")
target_include_directories(MidnightEngine PUBLIC "${CMAKE_SOURCE_DIR}/Source/Core")
target_include_directories(MidnightEngine PUBLIC "${CMAKE_SOURCE_DIR}/Source/UI")
target_include_directories(MidnightEngine PUBLIC "${THIRD_PARTY_INCLUDE_PATH}")

if(WIN32)

add_custom_command(TARGET MidnightEngine POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/Resources $<TARGET_FILE_DIR:MidnightEngine>/Resources)

elseif(APPLE)

find_library(FOUNDATION Foundation)
find_library(COCOA Cocoa)
find_library(IOKIT IoKit)
find_library(METAL Metal)
find_library(QUARTZCORE QuartzCore)

target_link_libraries(MidnightEngine ${FOUNDATION} ${COCOA} ${IOKIT} ${METAL} ${QUARTZCORE})
add_custom_command(TARGET MidnightEngine POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/Resources $<TARGET_FILE_DIR:MidnightEngine>/Resources)

elseif(UNIX AND NOT APPLE)

add_custom_command(TARGET MidnightEngine POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/Resources $<TARGET_FILE_DIR:MidnightEngine>/Resources)

else()

message(FATAL_ERROR "Unsupported OS.")

endif()

target_link_libraries(MidnightEngine debug ${THIRD_PARTY_DEBUG_LIBS})
target_link_libraries(MidnightEngine optimized ${THIRD_PARTY_RELEASE_LIBS})

foreach(LIBRARY ${THIRD_PARTY_DEBUG_DYLIBS})

    get_filename_component(FILENAME "${LIBRARY}" NAME)
    add_custom_command(TARGET MidnightEngine POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${LIBRARY} ${CMAKE_CURRENT_BINARY_DIR}/${FILENAME})

endforeach()