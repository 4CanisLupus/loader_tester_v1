cmake_minimum_required(VERSION 3.5)

project(loader_tester_v1 VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

set(PROJECT_SOURCES
        src/main.cpp
        src/logger.cpp
        include/logger.h
)


qt_add_executable(loader_tester_v1
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        #include/mainwindow.h src/main.cpp src/mainwindow.cpp
)
target_include_directories(loader_tester_v1 PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(loader_tester_v1 PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)


if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.loader_tester_v1)
endif()
set_target_properties(loader_tester_v1 PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS loader_tester_v1
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(loader_tester_v1)
endif()
