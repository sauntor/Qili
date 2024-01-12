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

if (WIN32)
    include(InstallRequiredSystemLibraries)
elseif(APPLE)
    message(FATAL_ERROR "Not tested on Apple's products, comment out this line if you want take the risk on your own!")
elseif(ANDROID)
    message(FATAL_ERROR "Not tested on Android yet, comment out this line if you want take the risk on your own!")
elseif (LINUX OR UNIX)
    cmake_host_system_information(RESULT DISTRO QUERY DISTRIB_INFO)

    configure_file(
        "${CMAKE_SOURCE_DIR}/Linux/me.sauntor.qili.desktop.in"
        "${CMAKE_BINARY_DIR}/me.sauntor.qili.desktop"
        @ONLY)
    install(FILES ${CMAKE_BINARY_DIR}/me.sauntor.qili.desktop
            DESTINATION ${CMAKE_INSTALL_DATADIR}/applications/)
else()
    message(FATAL_ERROR "Not tested on ${CMAKE_SYSTEM} yet, comment out this line if you want take the risk on your own!")
endif()
