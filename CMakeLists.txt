
cmake_minimum_required(VERSION 3.13)
project(std)

if(NOT DEFINED PLATFORM)
    set(PLATFORM "windows")
endif()

message(STATUS "Target platform: ${PLATFORM}")

find_program(CLANG_EXECUTABLE NAMES clang REQUIRED)
find_program(LLVM_LINK_EXECUTABLE NAMES llvm-link REQUIRED)
find_program(OPT_EXECUTABLE NAMES opt REQUIRED)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -emit-llvm -c")

set(COMMON_DIR "${CMAKE_SOURCE_DIR}/common")
set(PLATFORM_DIR "${CMAKE_SOURCE_DIR}/${PLATFORM}")

file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/bc)

file(GLOB COMMON_SOURCES "${COMMON_DIR}/*.cpp")
file(GLOB PLATFORM_SOURCES "${PLATFORM_DIR}/*.cpp")

set(BC_FILES "")
foreach(SRC ${COMMON_SOURCES} ${PLATFORM_SOURCES})
    get_filename_component(FILENAME ${SRC} NAME_WE)
    set(BC_FILE "${CMAKE_BINARY_DIR}/bc/${FILENAME}.bc")
    add_custom_command(
        OUTPUT ${BC_FILE}
        COMMAND ${CLANG_EXECUTABLE} -emit-llvm -c ${SRC} -o ${BC_FILE}
        DEPENDS ${SRC}
        COMMENT "Compiling ${SRC} to LLVM bitcode"
    )
    list(APPEND BC_FILES ${BC_FILE})
endforeach()

add_custom_command(
    OUTPUT ${CMAKE_BINARY_DIR}/std.bc
    COMMAND ${LLVM_LINK_EXECUTABLE} ${BC_FILES} -o ${CMAKE_BINARY_DIR}/std.bc
    DEPENDS ${BC_FILES}
    COMMENT "Linking all bitcode files into std.bc"
)

add_custom_command(
    OUTPUT ${CMAKE_BINARY_DIR}/std.opt.bc
    COMMAND ${OPT_EXECUTABLE} -O2 ${CMAKE_BINARY_DIR}/std.bc -o ${CMAKE_BINARY_DIR}/std.opt.bc
    DEPENDS ${CMAKE_BINARY_DIR}/std.bc
    COMMENT "Optimizing std.bc into std.opt.bc"
)

add_custom_target(std ALL
    DEPENDS ${CMAKE_BINARY_DIR}/std.opt.bc
    DEPENDS ${CMAKE_BINARY_DIR}/std.sb
)

add_custom_command(
    OUTPUT ${CMAKE_BINARY_DIR}/std.sb
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${CMAKE_SOURCE_DIR}/std.sb
            ${CMAKE_BINARY_DIR}/std.sb
    DEPENDS ${CMAKE_SOURCE_DIR}/std.sb
    COMMENT "Copying std header to build directory"
)
