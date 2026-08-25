cmake_minimum_required(VERSION 3.13)
project(std)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT DEFINED PLATFORM)
    if(WIN32)
        set(PLATFORM "windows")
    elseif(UNIX AND NOT APPLE)
        set(PLATFORM "linux")
    else()
        set(PLATFORM "windows")  # Default fallback
    endif()
endif()

message(STATUS "Target platform: ${PLATFORM}")

find_program(CLANG_EXECUTABLE NAMES clang REQUIRED)
find_program(LLVM_LINK_EXECUTABLE NAMES llvm-link REQUIRED)
find_program(OPT_EXECUTABLE NAMES opt REQUIRED)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

set(PLATFORM_DIR "${CMAKE_SOURCE_DIR}/${PLATFORM}")

file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/bc)

file(GLOB COMMON_SOURCES "common/*.cpp")
file(GLOB PLATFORM_SOURCES "${PLATFORM_DIR}/*.cpp")

set(BC_FILES "")
foreach(SRC ${COMMON_SOURCES} ${PLATFORM_SOURCES})
    get_filename_component(FILENAME ${SRC} NAME_WE)
    set(BC_FILE "${CMAKE_BINARY_DIR}/bc/${FILENAME}.bc")
    add_custom_command(
        OUTPUT ${BC_FILE}
        COMMAND ${CLANG_EXECUTABLE} -emit-llvm -nostdlib -c ${SRC} -o ${BC_FILE}
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

add_custom_target(dummy_bitcode ALL
    DEPENDS ${CMAKE_BINARY_DIR}/std.opt.bc
)

add_library(dummy_ide_support
    ${COMMON_SOURCES}
    ${PLATFORM_SOURCES}
)

add_custom_target(dummy_gen
    DEPENDS ${CMAKE_BINARY_DIR}/std.opt.bc
    COMMENT "Generate LLVM bitcode files"
)

add_custom_target(std
    COMMAND ;
    COMMENT "Build LLVM bitcode version of std library"
)
add_dependencies(std dummy_bitcode)