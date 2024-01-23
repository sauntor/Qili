# Copyright (C) 2024  Sauntor <sauntor@live.com>

# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.

# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.

# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <https://www.gnu.org/licenses/>.

cmake_minimum_required(VERSION 3.5)

project(
    Qili VERSION 1.1.0 LANGUAGES CXX
    HOMEPAGE_URL https://github.com/sauntor/Qili
    DESCRIPTION  "A subtitle spearker for live broadcasting at bilibili.com"
)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (CMAKE_BUILD_TYPE MATCHES "^[Rr]el|[Rr]el$")
    set(QILI_RELEASE_BUILD ON)
endif()

# OS Type for Qili
if (WIN32)
    set(QILI_ON_WIN32 ON)
endif()
if (APPLE)
    set(QILI_ON_APPLE ON)
endif()
if (LINUX)
    set(QILI_ON_LINUX ON)
    cmake_host_system_information(RESULT DISTRO QUERY DISTRIB_INFO)
endif()
if (UNIX)
    set(QILI_ON_UNIX ON)
endif()

option(DEV_MODE    "Enable developing mode?" OFF)
option(USE_QT5     "Force to build Qili using Qt5?" OFF)
option(USE_CPACK   "Use CPack for packaging?" OFF)
option(IFW_WITH_LIBS   "Build an IFW installer which contains Qt and 3rdparty libraries?" OFF)
set(QILI_PACKAGE_ID   "me.sauntor.qili" CACHE STRING "The `package` name of rpm/deb/ifw")

if (USE_CPACK)
    set(QILI_PACK_TYPE "CPACK")
    if (NOT QILI_INSTALL_PREFIX)
        set(QILI_INSTALL_PREFIX "/usr")
    endif()
endif()
if (USE_IFW)
    if (USE_CPACK)
        message(FATAL_ERROR "Please DO NOT use IFW and CPack together!")
    endif()
    set(QILI_PACK_TYPE "IFW")
    if (NOT QILI_INSTALL_PREFIX)
        set(QILI_INSTALL_PREFIX "/opt")
    endif()
    message(STATUS "QILI_INSTALL_PREFIX = ${QILI_INSTALL_PREFIX}")
endif()
if (DEFINED QILI_PACK_TYPE)
    message(WARNING "Enter ${QILI_PACK_TYPE} packaging mode")
    if (NOT QILI_RELEASE_BUILD)
        message(WARNING "Please use ${QILI_PACK_TYPE} packaging mode with \"Release\" build type")
    endif()
    if (IFW_WITH_LIBS)
        message(WARNING [[
Please ensure you are using Qt binaries built from sources or from qt.io !
DO NOT use the binaries shipped by the Linux vendor!
        ]])
    endif()
endif()


set(QILI_QT_DEPS
    Core
    Gui
    Network
    WebSockets
    TextToSpeech
    Widgets
    LinguistTools
    VirtualKeyboard
)
if (DEV_MODE OR NOT QILI_RELEASE_BUILD)
    list(APPEND QILI_QT_DEPS UiPlugin)
endif()

if (USE_QT5)
    message(CHECK_START "Detecting Qt5")
    find_package(QT NAMES Qt5 5.15 REQUIRED COMPONENTS ${QILI_QT_DEPS})
    find_package(Qt5 REQUIRED COMPONENTS ${QILI_QT_DEPS})
    if (NOT QT_FOUND)
        message(CHECK_FAIL "Not Found")
    else()
        message(CHECK_PASS "Using Qt ${QT_VERSION_MAJOR}.${QT_VERSION_MINOR}")
    endif()
else()
    message(CHECK_START "Detecting Qt5/Qt6")
    find_package(QT NAMES Qt6 REQUIRED COMPONENTS ${QILI_QT_DEPS})
    if (NOT QT_FOUND)
        find_package(QT NAMES Qt5 REQUIRED COMPONENTS ${QILI_QT_DEPS})
    endif()
    if (NOT QT_FOUND)
        message(CHECK_FAIL "Not Found")
    else()
        find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS ${QILI_QT_DEPS})
        message(CHECK_PASS "Using Qt ${QT_VERSION_MAJOR}.${QT_VERSION_MINOR}")
    endif()
endif()

if (QT_VERSION_MAJOR EQUAL 6)
    include("${Qt${QT_VERSION_MAJOR}_DIR}/FindWrapBrotli.cmake")
else()
    include(FindWrapBrotli.cmake)
endif()

function(qili_project_setup)
    if (${QT_VERSION_MAJOR} GREATER 5)
        qt_standard_project_setup()
    endif()
endfunction()

function(qili_add_executable name)
    if (QT_VERSION_MAJOR GREATER 5)
        qt_add_executable(${name} WIN32 MACOSX_BUNDLE MANUAL_FINALIZATION)
    else()
        add_executable(${name} WIN32 MACOSX_BUNDLE)
    endif()
endfunction()


function(qili_add_library name)
    if (QT_VERSION_MAJOR GREATER 5)
        qt_add_library(${name} SHARED MANUAL_FINALIZATION)
    else()
        add_library(${name} SHARED)
    endif()
endfunction()

function(qili_add_plugin name)
    if (QT_VERSION_MAJOR GREATER 5)
        qt_add_plugin(${name} SHARED MANUAL_FINALIZATION)
    else()
        add_library(${name} SHARED)
    endif()
endfunction()

function(qili_finalize_target name)
    if (${QT_VERSION_MAJOR} GREATER 5)
        qt_finalize_target(${name})
    endif()
endfunction()
function(qili_finalize_executable name)
    if (${QT_VERSION_MAJOR} GREATER 5)
        qt_finalize_executable(${name})
    endif()
endfunction()
qili_project_setup()

include(GNUInstallDirs)
if (NOT QILI_INSTALL_PREFIX)
    set(QILI_INSTALL_PREFIX ${CMAKE_INSTALL_PREFIX})
endif()
set(QILI_TRANSLATIONS_DIR "${CMAKE_INSTALL_DATADIR}/${QILI_PACKAGE_ID}/translations")

configure_file(Config.h.in ${CMAKE_BINARY_DIR}/Config.h @ONLY)

# Project Sub Modules
add_subdirectory(Thirdparty)
add_subdirectory(Widgets)
if (DEV_MODE OR NOT QILI_RELEASE_BUILD)
    add_subdirectory(Designer)
endif()
add_subdirectory(App)


include("${CMAKE_SOURCE_DIR}/QiliInstall.cmake")

if (USE_CPACK)
    configure_file ("${CMAKE_SOURCE_DIR}/QiliCPack.cmake"
                    "${CMAKE_BINARY_DIR}/QiliCPack.cmake"
                    @ONLY)
    set(CPACK_PROPERTIES_FILE "${CMAKE_BINARY_DIR}/QiliCPack.cmake")
    include(CPack)
endif()

if (USE_IFW)
    include("${CMAKE_SOURCE_DIR}/QiliIFW.cmake")
endif()
