cmake_minimum_required(VERSION 3.8)

cmake_policy(SET CMP0091 NEW)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG")
set(LINUX UNIX AND NOT APPLE)
set(CMAKE_DEBUG_POSTFIX d)

file(GLOB_RECURSE ENGINE_RESOURCES ${CMAKE_SOURCE_DIR}/Resources/*)
file(GLOB_RECURSE MODULES ${CMAKE_SOURCE_DIR}/Source/*.Module)

project(MidnightEngine VERSION 0.0.1 LANGUAGES C CXX)

include(${CMAKE_SOURCE_DIR}/Functions.cmake)

foreach(MODULE ${MODULES})

    include(${MODULE})

endforeach()

add_library(MidnightEngine SHARED ${ENGINE_SOURCES} ${ENGINE_HEADERS} ${ENGINE_INLINE} ${THIRD_PARTY_SOURCES} ${THIRD_PARTY_HEADERS})

target_compile_definitions(MidnightEngine PRIVATE "ENGINE_EXPORT=1")

set_target_properties(MidnightEngine PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/Build/)

filterEngineSources("${ENGINE_SOURCES}")
filterEngineSources("${ENGINE_HEADERS}")
filterEngineSources("${ENGINE_INLINE}")
filterEngineSources("${ENGINE_RESOURCES}")
filterThirdPartySources("${THIRD_PARTY_SOURCES}")
filterThirdPartySources("${THIRD_PARTY_HEADERS}")

target_include_directories(MidnightEngine PUBLIC "${CMAKE_SOURCE_DIR}/Source")
target_include_directories(MidnightEngine PUBLIC "${CMAKE_SOURCE_DIR}/Source/Public")
target_include_directories(MidnightEngine PUBLIC "${CMAKE_SOURCE_DIR}/Source/Private")

target_include_directories(MidnightEngine PUBLIC "${CMAKE_SOURCE_DIR}/Source/Public/Core")
target_include_directories(MidnightEngine PUBLIC "${CMAKE_SOURCE_DIR}/Source/Public/UI")
target_include_directories(MidnightEngine PUBLIC "${CMAKE_SOURCE_DIR}/Source/Private/Core")
target_include_directories(MidnightEngine PUBLIC "${CMAKE_SOURCE_DIR}/Source/Private/UI")

target_include_directories(MidnightEngine PUBLIC "${THIRD_PARTY_INCLUDE_PATH}")

if(WIN32)

target_link_libraries(MidnightEngine debug ${THIRD_PARTY_DEBUG_LIBS})
target_link_libraries(MidnightEngine optimized ${THIRD_PARTY_RELEASE_LIBS})
target_link_libraries(MidnightEngine dbghelp.lib)

elseif(APPLE)

find_library(FOUNDATION Foundation)
find_library(COCOA Cocoa)
find_library(IOKIT IoKit)
find_library(METAL Metal)
find_library(QUARTZCORE QuartzCore)

find_package(SDL2 REQUIRED)
find_package(FMT REQUIRED)
find_package(ASSIMP REQUIRED)

find_library(FMTLIB fmt REQUIRED)

target_link_libraries(MidnightEngine ${THIRD_PARTY_RELEASE_LIBS} ${FOUNDATION} ${COCOA} ${IOKIT} ${METAL} ${QUARTZCORE} ${SDL2_LIBRARIES} ${FMTLIB} ${ASSIMP_LIBRARIES})
set(THIRD_PARTY_INCLUDE_PATH ${THIRD_PARTY_INCLUDE_PATH} ${SDL2_INCLUDE_DIRS} ${FMT_INCLUDE_DIRS} ${ASSIMP_INCLUDE_DIRS})

elseif(LINUX)

set(THREADS_PREFER_PTHREAD_FLAG ON)
set(OpenGL_GL_PREFERENCE GLVND)

find_package(X11 REQUIRED)
find_package(Threads REQUIRED)
find_package(OpenGL REQUIRED)

find_package(SDL2 REQUIRED)
find_package(FMT REQUIRED)
find_package(assimp REQUIRED)

find_library(FMTLIB fmt REQUIRED)

set(THIRD_PARTY_INCLUDE_PATH ${THIRD_PARTY_INCLUDE_PATH} ${SDL2_INCLUDE_DIRS} ${FMT_INCLUDE_DIRS} ${ASSIMP_INCLUDE_DIRS})

target_link_libraries(MidnightEngine ${THIRD_PARTY_RELEASE_LIBS} ${SDL2_LIBRARIES} ${FMTLIB} ${ASSIMP_LIBRARIES} ${X11_LIBRARIES} Threads::Threads ${OPENGL_LIBRARIES} ${CMAKE_DL_LIBS})

endif()

add_custom_command(TARGET MidnightEngine POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/Resources $<TARGET_FILE_DIR:MidnightEngine>/Resources)

add_subdirectory("${CMAKE_SOURCE_DIR}/MidnightApp")

set_property(DIRECTORY ${CMAKE_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT MidnightApp)