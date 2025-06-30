cmake_minimum_required(VERSION 3.16)

project(ControleFinanceiro VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Dependências Qt
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Sql)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Sql)

# Fontes do projeto
set(PROJECT_SOURCES
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui

    models/conta.cpp
    models/conta.h
    models/transacao.cpp
    models/transacao.h
    models/usuario.cpp
    models/usuario.h

    controllers/gerenciador.cpp
    controllers/gerenciador.h
    controllers/bancodados.cpp
    controllers/bancodados.h
)

# Criação do executável
if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(ControleFinanceiro
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        models/despesa.h
    )
else()
    if(ANDROID)
        add_library(ControleFinanceiro SHARED ${PROJECT_SOURCES})
    else()
        add_executable(ControleFinanceiro ${PROJECT_SOURCES})
    endif()
endif()

# Diretórios de include
target_include_directories(ControleFinanceiro PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/controllers
    ${CMAKE_CURRENT_SOURCE_DIR}/models
)

# Vinculação com bibliotecas do Qt
target_link_libraries(ControleFinanceiro PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Sql
)

# Propriedades para Windows e macOS
if(${QT_VERSION} VERSION_LESS 6.1.0)
    set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.ControleFinanceiro)
endif()

set_target_properties(ControleFinanceiro PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

# Instalação
include(GNUInstallDirs)
install(TARGETS ControleFinanceiro
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# Finalização para Qt6
if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(ControleFinanceiro)
endif()
