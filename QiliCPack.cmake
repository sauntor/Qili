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

cmake_host_system_information(RESULT DISTRO QUERY DISTRIB_INFO)

set(CPACK_PACKAGE_NAME                  me.sauntor.qili)
set(CPACK_PACKAGE_VENDOR                "Sauntor OSS")
set(CPACK_PACKAGE_CONTACT               "Sauntor <sauntor@live.com>")
set(CPACK_PACKAGE_SUMMARY               "@Qili_DESCRIPTION@")
set(CPACK_PACKAGE_DESCRIPTION           "@Qili_DESCRIPTION@")
set(CPACK_PACKAGE_FILE_NAME             "${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION}-${DISTRO_ID}.${DISTRO_VERSION_ID}_${CMAKE_SYSTEM_PROCESSOR}")
set(CPACK_RESOURCE_FILE_LICENSE         "@CMAKE_SOURCE_DIR@/LICENSE")
set(CPACK_RESOURCE_FILE_README          "@CMAKE_SOURCE_DIR@/README.md")

set(QILI_INSTALL_PREFIX                  "@QILI_INSTALL_PREFIX@")
set(CPACK_PACKAGING_INSTALL_PREFIX       "@QILI_INSTALL_PREFIX@")

# RPM
set(CPACK_RPM_CHANGELOG_FILE              "@CMAKE_SOURCE_DIR@/changelog")
set(CPACK_RPM_POST_INSTALL_SCRIPT_FILE    "@CMAKE_SOURCE_DIR@/Linux/rpm/postinst")
set(CPACK_RPM_POST_UNINSTALL_SCRIPT_FILE  "@CMAKE_SOURCE_DIR@/Linux/rpm/postrm")
set(CPACK_RPM_PACKAGE_DESCRIPTION         "${CPACK_PACKAGE_SUMMARY}")

set(CPACK_RPM_PACKAGE_GROUP    "Applications/Internet")
set(CPACK_RPM_CHANGELOG_FILE   "@CMAKE_SOURCE_DIR@/changelog")

# DEB for ubuntu/kylin 22.04
set(CPACK_DEBIAN_PACKAGE_CONTROL_EXTRA
    "@CMAKE_SOURCE_DIR@/Linux/deb/postinst;@CMAKE_SOURCE_DIR@/Linux/deb/postrm"
)
set(CPACK_DEBIAN_PACKAGE_DEPENDS "libqt5widgets5")
string(APPEND CPACK_DEBIAN_PACKAGE_DEPENDS ", libqt5network5")
string(APPEND CPACK_DEBIAN_PACKAGE_DEPENDS ", libqt5websockets5")
string(APPEND CPACK_DEBIAN_PACKAGE_DEPENDS ", libqt5websockets5")
string(APPEND CPACK_DEBIAN_PACKAGE_DEPENDS ", libqt5texttospeech5")
set(CPACK_DEBIAN_PACKAGE_RECOMMENDS "speech-dispatcher")
string(APPEND CPACK_DEBIAN_PACKAGE_RECOMMENDS ", speech-dispatcher-espeak-ng")
string(APPEND CPACK_DEBIAN_PACKAGE_RECOMMENDS ", espeak-ng")
string(APPEND CPACK_DEBIAN_PACKAGE_RECOMMENDS ", mbrola")
