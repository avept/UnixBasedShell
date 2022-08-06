project(UnixShell)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -w") 

add_executable(ShellExec src/main.cpp src/shellFunctionality.cpp)