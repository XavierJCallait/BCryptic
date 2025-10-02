cmake_minimum_required(VERSION 3.31.2)
project(BCryptic VERSION 0.1.0 LANGUAGES CXX DESCRIPTION "A simple C++ password manager application")

# Set C++20 Standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Import packages
find_package(Qt6 REQUIRED COMPONENTS Core Widgets)
find_package(SQLite3 REQUIRED)

find_package(PkgConfig REQUIRED)
pkg_check_modules(SODIUM REQUIRED libsodium)

enable_testing()

# Handle Qt MOC, UIC, and RCC automatically
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# ---
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

    include/EnterMasterPage.h
    include/PasswordsMainWindow.h
    include/QuestionsPage.h
    include/SetMasterPage.h
    include/StartPage.h
    include/StartupMainWindow.h
)

target_link_libraries(BCryptic
    bcryptic_lib
    Qt6::Widgets
)

set_target_properties(BCryptic PROPERTIES
    WIN32_EXECUTABLE FALSE
    MACOSX_BUNDLE TRUE
)

find_library(GTEST_LIB gtest REQUIRED)
find_library(GTEST_MAIN_LIB gtest_main REQUIRED)

add_executable(runTests
    tests/test_vault.cpp
    tests/test_utils.cpp
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
# ---


# set(SOURCES
#     src/EnterMasterPage.cpp
#     src/database.cpp
#     src/QuestionsPage.cpp
#     src/SetMasterPage.cpp
#     src/utils.cpp
#     src/PasswordsMainWindow.cpp
#     src/StartPage.cpp
#     src/vault.cpp
#     src/StartupMainWindow.cpp
#     src/Secret.cpp
#     src/BuildPasswords.cpp
#     src/main.cpp
# )

# set(HEADERS
#     include/EnterMasterPage.h
#     include/database.h
#     include/QuestionsPage.h
#     include/SetMasterPage.h
#     include/utils.h
#     include/PasswordsMainWindow.h
#     include/StartPage.h
#     include/vault.h
#     include/StartUpMainWindow.h
#     include/Secret.h
#     include/BuildPasswords.h
# )

# # If you have .ui files, list them here
# # set(UI 
# #     inc/mywindow.ui
# # )

# # If you have resource files, list them here
# # set(RESOURCES
# #     resources.qrc
# # )

# add_executable(${PROJECT_NAME} 
#     ${SOURCES} 
#     ${HEADERS}
#     # ${UI}
#     # ${RESOURCES}
# )

# # Link Qt libraries
# target_link_libraries(${PROJECT_NAME} 
#     Qt6::Core
#     Qt6::Widgets
#     SQLite::SQLite3
#     ${SODIUM_LIBRARIES}
# )

# # Set target properties
# set_target_properties(${PROJECT_NAME} PROPERTIES
#     # WIN32_EXECUTABLE TRUE 
#     WIN32_EXECUTABLE FALSE
#     MACOSX_BUNDLE TRUE
# )

# target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic -Werror)

# target_include_directories(${PROJECT_NAME} PRIVATE
#     ${CMAKE_CURRENT_SOURCE_DIR}/src
#     ${CMAKE_CURRENT_SOURCE_DIR}/include
# )

# message(STATUS "Built using C++ standard ${CMAKE_CXX_STANDARD}")

# find_library(GTEST_LIB gtest REQUIRED)
# find_library(GTEST_MAIN_LIB gtest_main REQUIRED)

# # Add your test sources
# set(TEST_SOURCES
#     tests/test_BuildPasswords.cpp
#     # tests/.cpp
# )

# add_executable(runTests ${TEST_SOURCES})

# target_link_libraries(runTests
#     PRIVATE
#     ${GTEST_LIB}
#     ${GTEST_MAIN_LIB}
#     SQLite::SQLite3
#     ${SODIUM_LIBRARIES}
#     Qt6::Core
#     Qt6::Widgets
# )

# target_include_directories(runTests PRIVATE
#     ${CMAKE_CURRENT_SOURCE_DIR}/include
#     ${CMAKE_CURRENT_SOURCE_DIR}/src
# )

# add_test(NAME BCrypticTests COMMAND runTests)