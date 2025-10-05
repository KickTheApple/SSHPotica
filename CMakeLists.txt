cmake_minimum_required(VERSION 3.10)
project(SSHPotica C)

set(CMAKE_C_STANDARD 11)

find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBSSH REQUIRED libssh)

add_executable(SSHPotica main.c main.h)

include_directories(.)

target_include_directories(SSHPotica PRIVATE ${LIBSSH_INCLUDE_DIRS})
target_link_libraries(SSHPotica PRIVATE ${LIBSSH_LIBRARIES})
target_compile_options(SSHPotica PRIVATE ${LIBSSH_CFLAGS_OTHER})

