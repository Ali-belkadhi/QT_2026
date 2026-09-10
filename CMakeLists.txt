cmake_minimum_required(VERSION 3.16)

project(centre VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


# =====================================================
# QT MODULES
# =====================================================

find_package(
    QT
    NAMES Qt6 Qt5
    REQUIRED COMPONENTS
        Widgets
        Sql
        Charts
        Network
)

find_package(
    Qt${QT_VERSION_MAJOR}
    REQUIRED COMPONENTS
        Widgets
        Sql
        Charts
        Network
)


# =====================================================
# SOURCES PRINCIPAUX
# =====================================================

set(PROJECT_SOURCES
    main.cpp

    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    resources.qrc
)


# =====================================================
# CREATION EXECUTABLE
# =====================================================

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)

    qt_add_executable(centre
        MANUAL_FINALIZATION

        ${PROJECT_SOURCES}

        database.h
        database.cpp

        formateur.h
        formateur.cpp

        cours.h
        cours.cpp

        statformateur.h
        statformateur.cpp

        statcours.h
        statcours.cpp
        emailservice.h emailservice.cpp
        smtpclient.h smtpclient.cpp
        utilisateur.h utilisateur.cpp


    )

else()

    if(ANDROID)

        add_library(centre SHARED
            ${PROJECT_SOURCES}

            database.h
            database.cpp

            formateur.h
            formateur.cpp

            cours.h
            cours.cpp

            statformateur.h
            statformateur.cpp

            statcours.h
            statcours.cpp

            emailservice.h
            emailservice.cpp
            smtpclient.h
            smtpclient.cpp
        )

    else()

        add_executable(centre
            ${PROJECT_SOURCES}

            database.h
            database.cpp

            formateur.h
            formateur.cpp

            cours.h
            cours.cpp

            statformateur.h
            statformateur.cpp

            statcours.h
            statcours.cpp

            emailservice.h
            emailservice.cpp
            smtpclient.h
            smtpclient.cpp
        )

    endif()

endif()


# =====================================================
# LIBRAIRIES QT
# =====================================================

target_link_libraries(
    centre
    PRIVATE

    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Sql
    Qt${QT_VERSION_MAJOR}::Charts
    Qt${QT_VERSION_MAJOR}::Network
)


# =====================================================
# MACOS / IOS
# =====================================================

if(${QT_VERSION} VERSION_LESS 6.1.0)

    set(
        BUNDLE_ID_OPTION

        MACOSX_BUNDLE_GUI_IDENTIFIER
        com.example.centre
    )

endif()


set_target_properties(
    centre
    PROPERTIES

    ${BUNDLE_ID_OPTION}

    MACOSX_BUNDLE_BUNDLE_VERSION
    ${PROJECT_VERSION}

    MACOSX_BUNDLE_SHORT_VERSION_STRING
    ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}

    MACOSX_BUNDLE TRUE

    WIN32_EXECUTABLE TRUE
)


# =====================================================
# INSTALLATION
# =====================================================

include(GNUInstallDirs)


install(
    TARGETS centre

    BUNDLE DESTINATION .

    LIBRARY DESTINATION
    ${CMAKE_INSTALL_LIBDIR}

    RUNTIME DESTINATION
    ${CMAKE_INSTALL_BINDIR}
)


# =====================================================
# FINALISATION QT6
# =====================================================

if(QT_VERSION_MAJOR EQUAL 6)

    qt_finalize_executable(centre)

endif()