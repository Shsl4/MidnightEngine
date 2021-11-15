cmake_minimum_required(VERSION 3.8)

cmake_policy(SET CMP0091 NEW)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(MAIN_FILE ${CMAKE_CURRENT_SOURCE_DIR}/Source/GLFWDemo.cpp)
file(GLOB_RECURSE GLFW_RESOURCES ${CMAKE_CURRENT_SOURCE_DIR}/Resources/*)
file(GLOB_RECURSE MODULES ${CMAKE_CURRENT_SOURCE_DIR}/*.Module)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

project(GLFWDemo VERSION 0.0.1)

foreach(MODULE ${MODULES})

    include(${MODULE})

endforeach()

add_executable(GLFWDemo ${MAIN_FILE}
                        ${GLFW_CORE_SOURCES} 
                        ${GLFW_CORE_HEADERS} 
                        ${GLFW_RESOURCES}
                        ${ENGINE_SOURCES} 
                        ${ENGINE_HEADERS} 
                        ${THIRD_PARTY_SOURCES} 
                        ${THIRD_PARTY_HEADERS})

filterDemoSources("${MAIN_FILE}" "GLFWDemo")
filterDemoSources("${GLFW_CORE_HEADERS}" "GLFWDemo")
filterDemoSources("${GLFW_CORE_SOURCES}" "GLFWDemo")
filterDemoSources("${GLFW_RESOURCES}" "GLFWDemo")
filterEngineSources("${ENGINE_SOURCES}")
filterEngineSources("${ENGINE_HEADERS}")
filterThirdPartySources("${THIRD_PARTY_SOURCES}")
filterThirdPartySources("${THIRD_PARTY_HEADERS}")

target_include_directories(GLFWDemo PUBLIC "${CMAKE_SOURCE_DIR}\\Source")
target_include_directories(GLFWDemo PUBLIC "${CMAKE_SOURCE_DIR}\\Source\\Core")
target_include_directories(GLFWDemo PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}\\Source")
target_include_directories(GLFWDemo PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}\\Source\\Core")
target_include_directories(GLFWDemo PUBLIC "${THIRD_PARTY_INCLUDE_PATH}")

if(WIN32)

add_custom_command(TARGET GLFWDemo POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/Resources ${CMAKE_CURRENT_BINARY_DIR}/Resources)

elseif(APPLE)

find_library(FOUNDATION Foundation)
find_library(COCOA Cocoa)
find_library(IOKIT IoKit)

target_link_libraries(GLFWDemo ${FOUNDATION} ${COCOA} ${IOKIT})
add_custom_command(TARGET GLFWDemo POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/Resources $<TARGET_FILE_DIR:GLFWDemo>/Resources)

elseif(UNIX AND NOT APPLE)

add_custom_command(TARGET GLFWDemo POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/Resources $<TARGET_FILE_DIR:GLFWDemo>/Resources)

endif()

target_link_libraries(GLFWDemo debug ${THIRD_PARTY_DEBUG_LIBS} optimized ${THIRD_PARTY_RELEASE_LIBS})
