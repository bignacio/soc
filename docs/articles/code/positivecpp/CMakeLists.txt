cmake_minimum_required(VERSION 3.27)
project(jsonferry)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(FetchContent)

FetchContent_Declare(
    cpphttplib
    GIT_REPOSITORY https://github.com/yhirose/cpp-httplib
    GIT_TAG v0.15.1
)
FetchContent_MakeAvailable(cpphttplib)


FetchContent_Declare(
    cppkafka
    GIT_REPOSITORY https://github.com/mfontanini/cppkafka
    GIT_TAG v0.4.1
)
FetchContent_MakeAvailable(cppkafka)

add_executable(jsonferry jsonferry.cpp)
target_link_libraries(jsonferry PRIVATE httplib::httplib cppkafka)
