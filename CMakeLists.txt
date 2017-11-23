cmake_minimum_required (VERSION 3.5)

project (UnigineTestTask)

set(CMAKE_CXX_STANDARD 14)

add_executable(UnigineTestTask Main.cpp)

add_subdirectory(UrlStatisticsCollector)
add_subdirectory(Submodules)
add_subdirectory(UnitTests)

target_link_libraries(UnigineTestTask UrlStatisticsCollector)
