cmake_minimum_required(VERSION 3.21)

project(Sheafly
    VERSION 0.0.1
    DESCRIPTION "Offline PDF toolkit"
    LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "" FORCE)
endif()

# Qt tooling.
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets Test)

# QPDF: prefer its CMake package, fall back to pkg-config.
find_package(qpdf CONFIG QUIET)
if(qpdf_FOUND)
    set(SHEAFLY_QPDF qpdf::libqpdf)
else()
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(QPDF REQUIRED IMPORTED_TARGET libqpdf)
    set(SHEAFLY_QPDF PkgConfig::QPDF)
endif()

# --- Core: pure logic, no Qt, no network. ---
add_library(sheafly_core STATIC
    src/core/PdfDocument.cpp
    src/core/PdfDocument.h
)
target_include_directories(sheafly_core PUBLIC src)
target_link_libraries(sheafly_core PRIVATE ${SHEAFLY_QPDF})

# --- App: Qt UI on top of the core. ---
qt_add_executable(sheafly
    src/main.cpp
    src/ui/MainWindow.cpp
    src/ui/MainWindow.h
)
target_link_libraries(sheafly PRIVATE sheafly_core Qt6::Widgets)

# No console window on Windows release builds.
set_target_properties(sheafly PROPERTIES WIN32_EXECUTABLE $<CONFIG:Release>)

# --- Tests ---
enable_testing()

qt_add_executable(sheafly_tests tests/test_pdfdocument.cpp)
target_link_libraries(sheafly_tests PRIVATE sheafly_core Qt6::Test)
target_compile_definitions(sheafly_tests PRIVATE
    SHEAFLY_TEST_DATA_DIR="${CMAKE_CURRENT_SOURCE_DIR}/tests/data")

add_test(NAME sheafly_tests COMMAND sheafly_tests)
