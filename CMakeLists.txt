cmake_minimum_required(VERSION 3.16)

project(integration_crudd VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 REQUIRED COMPONENTS Widgets Sql PrintSupport Pdf)
find_package(Qt6 REQUIRED COMPONENTS Widgets Sql PrintSupport Pdf)

set(PROJECT_SOURCES
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
)

qt_add_executable(integration_crudd
    ${PROJECT_SOURCES}
    connection.h connection.cpp
    employee.h employee.cpp
    equipement.h equipement.cpp
    dechet.h dechet.cpp
)

target_link_libraries(integration_crudd PRIVATE
    Qt6::Widgets
    Qt6::Sql
    Qt6::PrintSupport
    Qt6::Pdf
)

qt_finalize_executable(integration_crudd)
