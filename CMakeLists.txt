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
    src/main.cpp
    src/entermaster.cpp
    src/database.cpp
    src/questions.cpp
    src/setmaster.cpp
    src/utils.cpp
    src/passwords.cpp
    src/start.cpp
    src/vault.cpp
)

set(HEADERS
    include/entermaster.h
    include/database.h
    include/questions.h
    include/setmaster.h
    include/utils.h
    include/passwords.h
    include/start.h
    include/vault.h
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