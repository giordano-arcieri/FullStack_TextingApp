cmake_minimum_required(VERSION 3.10)
project(server)


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(PkgConfig)
pkg_check_modules(Pistache REQUIRED IMPORTED_TARGET libpistache)


# Add source files
file(GLOB SOURCES ${WORKING_DIRECTORY}src/*.cpp)


# Create executable
add_executable(server ${SOURCES})
target_link_libraries(${PROJECT_NAME} PkgConfig::Pistache)
