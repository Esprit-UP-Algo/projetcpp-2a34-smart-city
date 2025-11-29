cmake_minimum_required(VERSION 3.16)

project(integration_crudd VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)



# Find required Qt6 components
find_package(Qt6 REQUIRED COMPONENTS 
    Core
    Widgets
    Sql
    PrintSupport
    Pdf
    Network
    Multimedia
    MultimediaWidgets
)

# Try to find Qt Charts (optional)
find_package(Qt6 QUIET COMPONENTS Charts)
if(Qt6Charts_FOUND)
    message(STATUS "Qt Charts found - charts will be available")
    set(CHARTS_AVAILABLE TRUE)
else()
    message(STATUS "Qt Charts not found - charts will be disabled")
    set(CHARTS_AVAILABLE FALSE)
endif()

# Try to find Qt WebEngine (optional)
find_package(Qt6 QUIET COMPONENTS WebEngine WebEngineWidgets)
if(Qt6WebEngine_FOUND AND Qt6WebEngineWidgets_FOUND)
    message(STATUS "Qt WebEngine found - map feature will be available")
    set(WEBENGINE_AVAILABLE TRUE)
else()
    message(STATUS "Qt WebEngine not found - map feature will be disabled")
    set(WEBENGINE_AVAILABLE FALSE)
endif()

# Try to find OpenCV (optional - for local face recognition)
find_package(OpenCV QUIET)
if(OpenCV_FOUND)
    message(STATUS "OpenCV found - local face recognition will be available")
    set(OPENCV_AVAILABLE TRUE)
else()
    message(STATUS "OpenCV not found - local face recognition will be disabled")
    message(STATUS "  Note: MXFace API (cloud) will still work without OpenCV")
    set(OPENCV_AVAILABLE FALSE)
endif()

set(PROJECT_SOURCES
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    notificationmanager.cpp
    notificationmanager.h
    connection.h connection.cpp
    employee.h employee.cpp
    equipement.h equipement.cpp
    dechet.h dechet.cpp
    loginwindow.cpp
    loginwindow.h
    passwordrecovery.cpp
    passwordrecovery.h
    facedialog.h
    facedialog_qt6.cpp
    faceenrolldialog.h
    faceenrolldialog_qt6.cpp
    mxfaceapi.cpp
    mxfaceapi.h
)

qt_add_executable(integration_crudd
    ${PROJECT_SOURCES}
    resources.qrc
)

target_link_libraries(integration_crudd PRIVATE
    Qt6::Widgets
    Qt6::Sql
    Qt6::PrintSupport
    Qt6::Pdf
    Qt6::Network
    Qt6::Multimedia
    Qt6::MultimediaWidgets
)

# Link Qt Charts only if available
if(CHARTS_AVAILABLE)
    target_link_libraries(integration_crudd PRIVATE Qt6::Charts)
    target_compile_definitions(integration_crudd PRIVATE CHARTS_AVAILABLE)
endif()

# Link Qt WebEngine only if available
if(WEBENGINE_AVAILABLE)
    target_link_libraries(integration_crudd PRIVATE Qt6::WebEngine Qt6::WebEngineWidgets)
    target_compile_definitions(integration_crudd PRIVATE WEBENGINE_AVAILABLE)
endif()

# Link OpenCV only if available
if(OPENCV_AVAILABLE)
    target_link_libraries(integration_crudd PRIVATE ${OpenCV_LIBS})
    target_include_directories(integration_crudd PRIVATE ${OpenCV_INCLUDE_DIRS})
    target_compile_definitions(integration_crudd PRIVATE OPENCV_AVAILABLE)
endif()
