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


##################################
# QR code generation library     #
##################################
qili_add_library(QRCodeGen)

target_include_directories(QRCodeGen PRIVATE
    ${CMAKE_BINARY_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}
)
target_sources(QRCodeGen PRIVATE
    ThirdpartyExports.h
    QiliQRCode.h
    QiliQRCode.cpp
    qrcodegen.hpp
    qrcodegen.cpp
)
target_link_libraries(QRCodeGen PRIVATE Qt::Core)
target_compile_definitions(QRCodeGen PRIVATE QILI_THIRDPARTY_LIBRARY)

install(TARGETS QRCodeGen
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    COMPONENT Runtime
)

qili_finalize_target(QRCodeGen)
