set(GOOGLE_TEST_ROOT Submodules/googletest/googletest)

include_directories(
  ${PROJECT_SOURCE_DIR}/${GOOGLE_TEST_ROOT}
  ${PROJECT_SOURCE_DIR}/${GOOGLE_TEST_ROOT}/include)

set(GOOGLE_TEST_SOURCE
  ${PROJECT_SOURCE_DIR}/${GOOGLE_TEST_ROOT}/src/gtest-all.cc)

foreach(_source ${GOOGLE_TEST_SOURCE})
  set_source_files_properties(${_source} PROPERTIES GENERATED 1)
endforeach()

set(CMAKE_THREAD_PREFER_PTHREAD TRUE)
find_package(Threads REQUIRED)
if(CMAKE_USE_PTHREADS_INIT)
  set(CMAKE_C_FLAGS ${CMAKE_C_FLAGS} "-pthread")
endif()

add_library(GoogleTest ${GOOGLE_TEST_SOURCE})

target_link_libraries(GoogleTest ${CMAKE_THREAD_LIBS_INIT})
