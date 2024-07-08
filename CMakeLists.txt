cmake_minimum_required(VERSION 3.5)

project(MessageViewer LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5 COMPONENTS Core Gui Qml Quick SerialPort REQUIRED)

set(SOURCES
    main.cpp
    serialreader.cpp
    messagemodel.cpp
)

set(HEADERS
    serialreader.h
    messagemodel.h
    message.h
)

add_executable(${PROJECT_NAME}
    ${SOURCES}
    ${HEADERS}
    qml.qrc
)

target_link_libraries(${PROJECT_NAME} Qt5::Core Qt5::Gui Qt5::Qml Qt5::Quick Qt5::SerialPort)

set_target_properties(${PROJECT_NAME} PROPERTIES
    AUTOMOC ON
    AUTOUIC ON
    AUTORCC ON
)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SOURCES} ${HEADERS})
qt5_add_resources(QT_RESOURCES qml.qrc)
