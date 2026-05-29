cmake_minimum_required(VERSION 3.16)

project(FileIndexer LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

function(enable_strict_warnings target_name)
    if(MSVC)
        target_compile_options(${target_name} PRIVATE /W4 /WX)
    else()
        target_compile_options(${target_name} PRIVATE -Wall -Wextra -Wpedantic -Werror)
    endif()
endfunction()

add_library(FileIndexerCore
    FileScanner.cpp
    FileSearch.cpp
    FileIndex.cpp
)

target_include_directories(FileIndexerCore PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

enable_strict_warnings(FileIndexerCore)

add_executable(FileIndexerApp
    main.cpp
)

target_link_libraries(FileIndexerApp PRIVATE
    FileIndexerCore
)

enable_strict_warnings(FileIndexerApp)

add_executable(FileIndexerTests
    FileScannerTests.cpp
)

target_link_libraries(FileIndexerTests PRIVATE
    FileIndexerCore
)

enable_strict_warnings(FileIndexerTests)

add_executable(FileSearchTests
    FileSearchTests.cpp
)

target_link_libraries(FileSearchTests PRIVATE
    FileIndexerCore
)

enable_strict_warnings(FileSearchTests)

add_executable(FileIndexTests
    FileIndexTests.cpp
)

target_link_libraries(FileIndexTests PRIVATE
    FileIndexerCore
)

enable_strict_warnings(FileIndexTests)

enable_testing()

add_test(NAME FileIndexerTests COMMAND FileIndexerTests)
add_test(NAME FileSearchTests COMMAND FileSearchTests)
add_test(NAME FileIndexTests COMMAND FileIndexTests)