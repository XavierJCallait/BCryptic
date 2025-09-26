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


# Handle Qt MOC, UIC, and RCC automatically
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

set(SOURCES
    src/EnterMasterPage.cpp
    src/database.cpp
    src/QuestionsPage.cpp
    src/SetMasterPage.cpp
    src/utils.cpp
    src/PasswordsMainWindow.cpp
    src/StartPage.cpp
    src/vault.cpp
    src/StartupMainWindow.cpp
    src/Secret.cpp
    src/main.cpp
)

set(HEADERS
    include/EnterMasterPage.h
    include/database.h
    include/QuestionsPage.h
    include/SetMasterPage.h
    include/utils.h
    include/PasswordsMainWindow.h
    include/StartPage.h
    include/vault.h
    include/StartUpMainWindow.h
    include/Secret.h
)

# If you have .ui files, list them here
# set(UI 
#     inc/mywindow.ui
# )

# If you have resource files, list them here
# set(RESOURCES
#     resources.qrc
# )

add_executable(${PROJECT_NAME} 
    ${SOURCES} 
    ${HEADERS}
    # ${UI}
    # ${RESOURCES}
)

# Link Qt libraries
target_link_libraries(${PROJECT_NAME} 
    Qt6::Core
    Qt6::Widgets
    SQLite::SQLite3
    ${SODIUM_LIBRARIES}
)

# Set target properties
set_target_properties(${PROJECT_NAME} PROPERTIES
    # WIN32_EXECUTABLE TRUE 
    WIN32_EXECUTABLE FALSE
    MACOSX_BUNDLE TRUE
)

target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic -Werror)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

message(STATUS "Built using C++ standard ${CMAKE_CXX_STANDARD}")