project(MidnightApp VERSION 0.0.1 LANGUAGES C CXX)

file(GLOB_RECURSE APP_SOURCES ${CMAKE_CURRENT_LIST_DIR}/*.c*)
file(GLOB_RECURSE APP_HEADERS ${CMAKE_CURRENT_LIST_DIR}/*.h*)
file(GLOB_RECURSE APP_INLINE ${CMAKE_CURRENT_LIST_DIR}/*.inl)

add_executable(MidnightApp ${APP_SOURCES} ${APP_HEADERS} ${APP_INLINE})

filterAppSources("${APP_SOURCES}")
filterAppSources("${APP_HEADERS}")
filterAppSources("${APP_INLINE}")

target_include_directories(MidnightApp PUBLIC "${CMAKE_CURRENT_LIST_DIR}/")
target_include_directories(MidnightApp PUBLIC "${CMAKE_SOURCE_DIR}/Source/Public")
target_include_directories(MidnightApp PUBLIC "${CMAKE_SOURCE_DIR}/Source/Public/Core")
target_include_directories(MidnightApp PUBLIC "${CMAKE_SOURCE_DIR}/Source/Public/UI")

set(EXECUTABLE_OUTPUT_PATH  ${CMAKE_SOURCE_DIR}/Build/)
set_property(TARGET MidnightApp PROPERTY VS_DEBUGGER_WORKING_DIRECTORY $<TARGET_FILE_DIR:MidnightApp>)

if(WIN32)

add_custom_command(TARGET MidnightApp POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/Resources $<TARGET_FILE_DIR:MidnightApp>/Resources)
target_link_libraries(MidnightApp PUBLIC MidnightEngine)

elseif(APPLE)

find_library(FOUNDATION Foundation)
find_library(COCOA Cocoa)
find_library(IOKIT IoKit)
find_library(METAL Metal)
find_library(QUARTZCORE QuartzCore)

target_link_libraries(MidnightApp MidnightEngine ${FOUNDATION} ${COCOA} ${IOKIT} ${METAL} ${QUARTZCORE})
add_custom_command(TARGET MidnightApp POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/Resources $<TARGET_FILE_DIR:MidnightApp>/Resources)

elseif(LINUX)

set(THREADS_PREFER_PTHREAD_FLAG ON)
set(OpenGL_GL_PREFERENCE GLVND)

find_package(SDL2 REQUIRED)
find_package(X11 REQUIRED)
find_package(Threads REQUIRED)
find_package(OpenGL REQUIRED)

set(THIRD_PARTY_INCLUDE_PATH ${THIRD_PARTY_INCLUDE_PATH} ${SDL2_INCLUDE_DIRS})

target_link_libraries(MidnightApp MidnightEngine ${SDL2_LIBRARIES} ${X11_LIBRARIES} Threads::Threads ${OPENGL_LIBRARIES} ${CMAKE_DL_LIBS})

add_custom_command(TARGET MidnightApp POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/Resources $<TARGET_FILE_DIR:MidnightApp>/Resources)

endif()

foreach(LIBRARY ${THIRD_PARTY_DEBUG_DYLIBS})

    get_filename_component(FILENAME "${LIBRARY}" NAME)
    add_custom_command(TARGET MidnightApp POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${LIBRARY} $<TARGET_FILE_DIR:MidnightApp>)

endforeach()
