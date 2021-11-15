cmake_minimum_required(VERSION 3.8)

cmake_policy(SET CMP0091 NEW)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(MAIN_FILE ${CMAKE_CURRENT_SOURCE_DIR}/Source/BGFXDemo.cpp)

file(GLOB_RECURSE BGFX_RESOURCES ${CMAKE_CURRENT_SOURCE_DIR}/Resources/*)
file(GLOB_RECURSE MODULES ${CMAKE_CURRENT_SOURCE_DIR}/*.Module)

project(BGFXDemo VERSION 0.0.1 LANGUAGES C CXX Swift)

foreach(MODULE ${MODULES})

    include(${MODULE})

endforeach()

add_executable(BGFXDemo ${MAIN_FILE}
                        ${BGFX_CORE_SOURCES} 
                        ${BGFX_CORE_HEADERS} 
                        ${BGFX_UI_SOURCES} 
                        ${BGFX_UI_HEADERS} 
                        ${BGFX_RESOURCES}
                        ${ENGINE_SOURCES} 
                        ${ENGINE_HEADERS} 
                        ${THIRD_PARTY_SOURCES} 
                        ${THIRD_PARTY_HEADERS})

filterDemoSources("${MAIN_FILE}" "BGFXDemo")
filterDemoSources("${BGFX_CORE_SOURCES}" "BGFXDemo")
filterDemoSources("${BGFX_CORE_HEADERS}" "BGFXDemo")
filterDemoSources("${BGFX_UI_SOURCES}" "BGFXDemo")
filterDemoSources("${BGFX_UI_HEADERS}" "BGFXDemo")
filterDemoSources("${BGFX_RESOURCES}" "BGFXDemo")
filterEngineSources("${ENGINE_SOURCES}")
filterEngineSources("${ENGINE_HEADERS}")
filterThirdPartySources("${THIRD_PARTY_SOURCES}")
filterThirdPartySources("${THIRD_PARTY_HEADERS}")

target_include_directories(BGFXDemo PUBLIC "${CMAKE_SOURCE_DIR}/Source")
target_include_directories(BGFXDemo PUBLIC "${CMAKE_SOURCE_DIR}/Source/Core")
target_include_directories(BGFXDemo PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/Source")
target_include_directories(BGFXDemo PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/Source/Core")
target_include_directories(BGFXDemo PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/Source/UI")
target_include_directories(BGFXDemo PUBLIC "${THIRD_PARTY_INCLUDE_PATH}")

if(WIN32)

add_custom_command(TARGET BGFXDemo POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/Resources ${CMAKE_CURRENT_BINARY_DIR}/Resources)

elseif(APPLE)

find_library(FOUNDATION Foundation)
find_library(COCOA Cocoa)
find_library(IOKIT IoKit)
find_library(METAL Metal)
find_library(QUARTZCORE QuartzCore)

target_link_libraries(BGFXDemo ${FOUNDATION} ${COCOA} ${IOKIT} ${METAL} ${QUARTZCORE})
add_custom_command(TARGET BGFXDemo POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/Resources $<TARGET_FILE_DIR:BGFXDemo>/Resources)

elseif(UNIX AND NOT APPLE)

add_custom_command(TARGET BGFXDemo POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/Resources $<TARGET_FILE_DIR:BGFXDemo>/Resources)

endif()

target_link_libraries(BGFXDemo debug ${THIRD_PARTY_DEBUG_LIBS} optimized ${THIRD_PARTY_RELEASE_LIBS})

foreach(LIBRARY ${THIRD_PARTY_DEBUG_DYLIBS})

    get_filename_component(FILENAME "${LIBRARY}" NAME)
    add_custom_command(TARGET BGFXDemo POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${LIBRARY} ${CMAKE_CURRENT_BINARY_DIR}/${FILENAME})

endforeach()