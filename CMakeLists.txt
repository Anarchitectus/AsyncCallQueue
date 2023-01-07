cmake_minimum_required (VERSION 3.8)

project ("AsyncCallQueue")

cmake_minimum_required (VERSION 3.8)  

SET(CMAKE_BUILD_TYPE Debug)
IF (WIN32)
    # set stuff for windows
ELSE()
    SET(CMAKE_CXX_COMPILER /usr/bin/clang++)
ENDIF()

add_executable (TestAsyncCallQueue Test/TestAsyncCallQueue.cpp AsyncInvokable.hpp AsyncCallQueue.hpp ConcurrentDeque.hpp)
add_executable (mainAsyncCallQueue main.cpp AsyncInvokable.hpp AsyncCallQueue.hpp ConcurrentDeque.hpp)

IF (WIN32)
  # set stuff for windows
ELSE()
  target_link_libraries(TestAsyncCallQueue pthread)
  target_link_libraries(mainAsyncCallQueue pthread)
ENDIF()

if (CMAKE_VERSION VERSION_GREATER 3.12)
 set_property(TARGET TestAsyncCallQueue PROPERTY CXX_STANDARD 20)
 set_property(TARGET mainAsyncCallQueue PROPERTY CXX_STANDARD 17)
endif()