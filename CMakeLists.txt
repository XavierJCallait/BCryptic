cmake_minimum_required(VERSION 3.31.2)
project(BCryptic VERSION 0.1.0 LANGUAGES CXX DESCRIPTION "A simple C++ password manager application")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(Qt6 REQUIRED COMPONENTS Core Widgets)
find_package(SQLite3 REQUIRED)

find_package(PkgConfig REQUIRED)
pkg_check_modules(SODIUM REQUIRED libsodium)

enable_testing()

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

add_library(bcryptic_lib
    src/BuildPasswords.cpp
    src/Secret.cpp
    src/database.cpp
    src/utils.cpp
    src/vault.cpp
)

target_include_directories(bcryptic_lib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(bcryptic_lib
    Qt6::Core
    SQLite::SQLite3
    ${SODIUM_LIBRARIES}
)

target_compile_options(bcryptic_lib PRIVATE -Wall -Wextra -Wpedantic -Werror)

add_executable(BCryptic
    src/main.cpp
    src/EnterMasterPage.cpp
    src/PasswordsMainWindow.cpp
    src/QuestionsPage.cpp
    src/SetMasterPage.cpp
    src/StartPage.cpp
    src/StartupMainWindow.cpp
    src/NewItemDialog.cpp
    src/NewDatabaseSplitter.cpp
    src/NewPasswordDialog.cpp

    include/EnterMasterPage.h
    include/PasswordsMainWindow.h
    include/QuestionsPage.h
    include/SetMasterPage.h
    include/StartPage.h
    include/StartupMainWindow.h
    include/NewItemDialog.h
    include/NewDatabaseSplitter.h
    include/NewPasswordDialog.h

    resources.qrc
)

target_link_libraries(BCryptic
    bcryptic_lib
    Qt6::Widgets
)

set_target_properties(BCryptic PROPERTIES
    WIN32_EXECUTABLE FALSE # Set to true to remove logging
    MACOSX_BUNDLE TRUE
)

find_library(GTEST_LIB gtest REQUIRED)
find_library(GTEST_MAIN_LIB gtest_main REQUIRED)

add_executable(runTests
    tests/test_vault.cpp
    tests/test_utils.cpp
    tests/test_BuildPasswords.cpp
)

target_link_libraries(runTests
    bcryptic_lib
    ${GTEST_LIB}
    ${GTEST_MAIN_LIB}
    Qt6::Widgets
)

target_include_directories(runTests PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

add_test(NAME BCrypticTests COMMAND runTests)
