enable_testing()

include_directories(${PROJECT_SOURCE_DIR}/Submodules/googletest/googletest/include)
  
add_executable(Tests UnitTests.cpp)

target_link_libraries(Tests GoogleTest UrlStatisticsCollector)