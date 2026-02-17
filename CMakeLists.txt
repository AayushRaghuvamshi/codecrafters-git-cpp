cmake_minimum_required(VERSION 3.14)

project(codecrafters-git)

set(CMAKE_CXX_STANDARD 23) # Enable the C++23 standard

file(GLOB_RECURSE SOURCE_FILES src/*.cpp src/*.hpp)
include(FetchContent)
FetchContent_Declare(ZStrGitRepo
  GIT_REPOSITORY    "https://github.com/mateidavid/zstr"
  GIT_TAG           "master"
)
FetchContent_MakeAvailable(ZStrGitRepo)

find_package(OpenSSL REQUIRED)
find_package(ZLIB REQUIRED)

add_executable(git ${SOURCE_FILES})

target_link_libraries(git PRIVATE OpenSSL::Crypto)
target_link_libraries(git PRIVATE ZLIB::ZLIB)
target_link_libraries(git PRIVATE zstr::zstr)