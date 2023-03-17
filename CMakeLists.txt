cmake_minimum_required(VERSION 3.0)
project(apl)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lpistache")


find_package(PkgConfig)
pkg_check_modules(Pistache REQUIRED IMPORTED_TARGET libpistache)


find_package(OpenCV REQUIRED)
if (${OpenCV_VERSION} VERSION_LESS 4.5.2)
  message(FATAL_ERROR "OpenCV version must be >= 4.5.2")
endif()
include_directories(${OpenCV_INCLUDE_DIRS})

find_package(nlohmann_json REQUIRED)



add_executable(${PROJECT_NAME} server.cpp)
target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS} ${OpenCL_LIBRARIES} PkgConfig::Pistache nlohmann_json)





