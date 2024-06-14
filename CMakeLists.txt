#==========================================================
#
#   ██████╗ ██╗   ██╗██╗██╗     ██████╗
#   ██╔══██╗██║   ██║██║██║     ██╔══██╗
#   ██████╔╝██║   ██║██║██║     ██║  ██║
#   ██╔══██╗██║   ██║██║██║     ██║  ██║
#   ██████╔╝╚██████╔╝██║███████╗██████╔╝
#   ╚═════╝  ╚═════╝ ╚═╝╚══════╝╚═════╝
#
#
#   Cmake File for combiling the project and running it.
#   The file is authored by: amir.kedis
#==========================================================

cmake_minimum_required(VERSION 3.12)
project(CRougeLite)

# DEPENDENCIES
set(RAYLIB_VERSION 5.0)
find_package(raylib ${RAYLIB_VERSION} QUIET)

# NOTE: the following script suposed to download raylib
#       and compile it if you don't have it
#       though I didn't test yet - amir
if (NOT raylib_FOUND)
  include(FetchContent)
  FetchContent_Declare(
    raylib
    DOWNLOAD_EXTRACT_TIMESTAMP OFF
    URL https://github.com/raysan5/raylib/archive/refs/tags/${RAYLIB_VERSION}.tar.gz
  )
  FetchContent_GetProperties(raylib)
  if (NOT raylib_POPULATED) # Have we downloaded raylib yet?
    set(FETCHCONTENT_QUIET NO)
    FetchContent_Populate(raylib)
    set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE) # don't build the supplied examples
    add_subdirectory(${raylib_SOURCE_DIR} ${raylib_BINARY_DIR})
  endif()
endif()

# PROJECT SETUP
add_executable(${PROJECT_NAME})
add_subdirectory(src) # NOTE: tells cmake to look for another cmake file in src dir

# NOTE: this will build the binary to projectname/projectname.exe
set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${PROJECT_NAME})

# NOTE: this copies the resources from src/resources to be at build folder
add_custom_command(
	TARGET ${PROJECT_NAME} POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/src/resources $<TARGET_FILE_DIR:${PROJECT_NAME}>/resources
	DEPENDS ${PROJECT_NAME})

# NOTE: link raylib
target_link_libraries(${PROJECT_NAME} raylib)

# Web Configurations
# NOTE: just no that it exist I don't know how to run it yet.
if (${PLATFORM} STREQUAL "Web")
    set_target_properties(${PROJECT_NAME} PROPERTIES SUFFIX ".html")
endif()



#==========================================================
#
#
#      ███████╗ ██████╗ ███████╗
#      ██╔════╝██╔═══██╗██╔════╝
#      █████╗  ██║   ██║█████╗  
#      ██╔══╝  ██║   ██║██╔══╝  
#      ███████╗╚██████╔╝██║     
#      ╚══════╝ ╚═════╝ ╚═╝     
#
#
#==========================================================
