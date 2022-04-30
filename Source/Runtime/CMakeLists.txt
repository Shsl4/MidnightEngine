project(Runtime VERSION 0.0.1 LANGUAGES C CXX)

file(GLOB_RECURSE RUNTIME_SOURCES ${CMAKE_CURRENT_LIST_DIR}/*.c*)
file(GLOB_RECURSE RUNTIME_HEADERS ${CMAKE_CURRENT_LIST_DIR}/*.h*)
file(GLOB_RECURSE RUNTIME_INLINE ${CMAKE_CURRENT_LIST_DIR}/*.inl)

add_executable(Runtime ${RUNTIME_SOURCES} ${RUNTIME_HEADERS} ${RUNTIME_INLINE})
target_link_libraries(Runtime PUBLIC MidnightEngine)

filterRuntimeSources("${RUNTIME_SOURCES}")
filterRuntimeSources("${RUNTIME_HEADERS}")
filterRuntimeSources("${RUNTIME_INLINE}")

target_include_directories(Runtime PUBLIC "${CMAKE_CURRENT_LIST_DIR}/")
target_include_directories(Runtime PUBLIC "${CMAKE_SOURCE_DIR}/Source")
target_include_directories(Runtime PUBLIC "${CMAKE_SOURCE_DIR}/Source/Core")
target_include_directories(Runtime PUBLIC "${CMAKE_SOURCE_DIR}/Source/UI")
target_include_directories(Runtime PUBLIC "${THIRD_PARTY_INCLUDE_PATH}")

if(WIN32)

add_custom_command(TARGET Runtime POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/Resources $<TARGET_FILE_DIR:Runtime>/../Resources)
add_custom_command(TARGET Runtime POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:MidnightEngine> $<TARGET_FILE_DIR:Runtime>)

elseif(APPLE)

find_library(FOUNDATION Foundation)
find_library(COCOA Cocoa)
find_library(IOKIT IoKit)
find_library(METAL Metal)
find_library(QUARTZCORE QuartzCore)

target_link_libraries(Runtime ${FOUNDATION} ${COCOA} ${IOKIT} ${METAL} ${QUARTZCORE})
add_custom_command(TARGET Runtime POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/Resources $<TARGET_FILE_DIR:Runtime>/Resources)

elseif(LINUX)

set(THREADS_PREFER_PTHREAD_FLAG ON)
set(OpenGL_GL_PREFERENCE GLVND)

find_package(SDL2 REQUIRED)
find_package(X11 REQUIRED)
find_package(Threads REQUIRED)
find_package(OpenGL REQUIRED)

set(THIRD_PARTY_INCLUDE_PATH ${THIRD_PARTY_INCLUDE_PATH} ${SDL2_INCLUDE_DIRS})

target_link_libraries(Runtime ${SDL2_LIBRARIES} ${X11_LIBRARIES} Threads::Threads ${OPENGL_LIBRARIES} ${CMAKE_DL_LIBS})

add_custom_command(TARGET Runtime POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/Resources $<TARGET_FILE_DIR:Runtime>/Resources)

else()

message(FATAL_ERROR "Unsupported OS.")

endif()

foreach(LIBRARY ${THIRD_PARTY_DEBUG_DYLIBS})

    get_filename_component(FILENAME "${LIBRARY}" NAME)
    add_custom_command(TARGET Runtime POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${LIBRARY} ${CMAKE_CURRENT_BINARY_DIR}/${FILENAME})

endforeach()