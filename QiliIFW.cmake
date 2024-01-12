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

## Variables for building
set(QILI_IFW_SRC "${CMAKE_SOURCE_DIR}/IFW")
set(QILI_IFW_BIN "${CMAKE_BINARY_DIR}/IFW")
set(QILI_IFW_PKG "packages/me.sauntor.qili")
# Change the install directory to IFW package's data dir
set(CMAKE_INSTALL_PREFIX "${QILI_IFW_BIN}/${QILI_IFW_PKG}/data")
set(QILI_IFW_EXT "${CMAKE_EXECUTABLE_SUFFIX}")

if (NOT QILI_IFW_EXT)
    set(QILI_IFW_EXT ".bin")
endif()

## Find IFW binaries
if (DEFINED IFW_PATH)
    set(QILI_IFW_PREFIX ${IFW_PATH})
elseif(NOT $ENV{IFW_PATH})
    set(QILI_IFW_PREFIX $ENV{IFW_PATH})
endif()
if (NOT QILI_IFW_PREFIX)
    message(WARNING "Can't find IFW binaries")
endif()

find_program(QILI_BINARY_CREATOR
    NAMES binarycreator binarycreator.exe binarycreator.app
    HINTS "${QILI_IFW_PREFIX}/bin"
)
find_program(QILI_INSTALLER_BASE
    NAMES installerbase installerbase.exe installerbase.app
    HINTS "${QILI_IFW_PREFIX}/bin"
)

## Custom commands to build the installer
add_custom_command(OUTPUT Qili_IFW_INIT
    COMMAND cmake -E make_directory "${CMAKE_INSTALL_PREFIX}"
)
add_custom_command(OUTPUT Qili_IFW_CLEAN
    COMMAND cmake -E remove_directory "${CMAKE_INSTALL_PREFIX}"
)

add_custom_command(OUTPUT Qili_IFW_copycfg
    COMMAND cmake -E copy_directory
    "${QILI_IFW_SRC}"
    "${QILI_IFW_BIN}"
    DEPENDS Qili_IFW_INIT
)
add_custom_command(OUTPUT Qili_IFW_copylic
    COMMAND cmake -E copy
    "${CMAKE_SOURCE_DIR}/LICENSE"
    "${QILI_IFW_BIN}/${QILI_IFW_PKG}/meta"
    DEPENDS Qili_IFW_copycfg
)

add_custom_command(OUTPUT Qili_IFW_instdata
    COMMAND cmake
    --build .
    --target install
    DEPENDS Qili_IFW_copycfg
    USES_TERMINAL
    VERBATIM
)

add_custom_command(OUTPUT Qili_IFW_createbin
    COMMAND "${QILI_BINARY_CREATOR}"
    -v
    --offline-only
    -t "${QILI_INSTALLER_BASE}"
    -c "${QILI_IFW_BIN}/config/general.xml"
    -p "packages"
    "${CMAKE_BINARY_DIR}/${PROJECT_NAME}-${PROJECT_VERSION}-${CMAKE_SYSTEM_NAME}-${CMAKE_SYSTEM_PROCESSOR}${QILI_IFW_EXT}"
    WORKING_DIRECTORY "${QILI_IFW_BIN}"
    DEPENDS Qili_IFW_copylic Qili_IFW_instdata
    USES_TERMINAL
    VERBATIM
)
add_custom_target(Qili_IFW_lupdate
    COMMAND Qt${QT_VERSION_MAJOR}::lupdate
    ${QILI_IFW_SRC}/config/controller.qs
    ${QILI_IFW_SRC}/${QILI_IFW_PKG}/meta/desktop.qs
    -ts
    ${QILI_IFW_SRC}/${QILI_IFW_PKG}/meta/zh_CN.ts
)
add_custom_target(Qili_IFW_lrelease
    COMMAND Qt${QT_VERSION_MAJOR}::lrelease
    ${QILI_IFW_SRC}/${QILI_IFW_PKG}/meta/zh_CN.ts
    -qm
    ${QILI_IFW_BIN}/${QILI_IFW_PKG}/meta/zh_CN.qm
    DEPENDS Qili_IFW_copycfg
)

add_custom_target(ifw
    DEPENDS
        Qili_IFW_INIT
        Qili_IFW_copycfg
        Qili_IFW_copylic
        Qili_IFW_instdata
        Qili_IFW_createbin
        # Qili_IFW_lrelease
        Qili_IFW_CLEAN
)
