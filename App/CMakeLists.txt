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

################################
# Main executable for Qili     #
################################

set(APP_TS_FILES QiliApp_zh_CN.ts)
set(APP_RC_FILES QiliApp.qrc)
set(APP_SRC_FILES
    QiliGlobal.h
    QiliAppGlobal.h
    Utility.h
    Utility.cpp

    QiliCookieJar.h
    QiliCookieJar.cpp
    QiliHttp.h
    QiliHttp.cpp

    QiliProtocol.h
    QiliProtocol.cpp
    QiliSocket.h
    QiliSocket.cpp

    QiliSettings.h
    QiliSettings.cpp
    QiliSpeaker.h
    QiliSpeaker.cpp
    QiliConnection.h
    QiliConnection.cpp

    QiliSettingsDialog.h
    QiliSettingsDialog.cpp
    QiliSettingsDialog.ui
    QiliLogin.h
    QiliLogin.cpp
    QiliLogin.ui
    QiliSubtitleLogger.h
    QiliSubtitleLogger.cpp
    QiliSubtitleLogger.ui
    QiliThanksDialog.h
    QiliThanksDialog.cpp
    QiliThanksDialog.ui

    QiliTray.h
    QiliTray.cpp

    QiliApp.h
    QiliApp.cpp

    main.cpp
    QiliLogger.h
    QiliLogger.cpp
)


qili_add_executable(QiliApp)
target_sources(QiliApp PRIVATE
    ${APP_SRC_FILES}
    ${APP_RC_FILES}
    ${APP_TS_FILES}
)
if (WIN32)
    target_sources(QiliApp PRIVATE Qili.rc)
    set_target_properties(QiliApp PROPERTIES OUTPUT_NAME Qili)
endif()

target_compile_definitions(QiliApp PRIVATE QILI_APP_LIBRARY)
add_dependencies(QiliApp QRCodeGen QiliWidgets)

if (QT_VERSION_MAJOR EQUAL 5)
    qt5_create_translation(APP_QM_FILES ${APP_SRC_FILES} ${APP_TS_FILES})
    add_custom_target(QiliApp_lrelease ALL DEPENDS ${APP_QM_FILES})
else()
    qt_add_translations(QiliApp TS_FILES ${APP_TS_FILES} QM_FILES_OUTPUT_VARIABLE APP_QM_FILES)
endif()

target_link_libraries(QiliApp
    PRIVATE
        Qt::Core
        Qt::Network
        Qt::WebSockets
        Qt::TextToSpeech
        Qt::Widgets
        WrapBrotli::WrapBrotliDec
        $<TARGET_LINKER_FILE:QRCodeGen>
        $<TARGET_LINKER_FILE:QiliWidgets>
)

target_include_directories(QiliApp PRIVATE ${CMAKE_BINARY_DIR})
target_include_directories(QiliApp PRIVATE ${CMAKE_SOURCE_DIR}/Thirdparty)
target_include_directories(QiliApp PRIVATE ${CMAKE_SOURCE_DIR}/Widgets)

if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER me.sauntor.Qili)
endif()
set_target_properties(QiliApp PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)


install(TARGETS QiliApp
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    COMPONENT Runtime
)

install(FILES ../Widgets/images/qili.png DESTINATION ${CMAKE_INSTALL_DATADIR}/pixmaps)
install(FILES ${APP_QM_FILES} DESTINATION ${CMAKE_INSTALL_DATADIR}/${QILI_PACKAGE_ID}/translations)

qili_finalize_executable(QiliApp)

if (WIN32 OR APPLE)
    set(BROTLI_LIB_COM "${CMAKE_SHARED_LIBRARY_PREFIX}brotlicommon${CMAKE_SHARED_LIBRARY_SUFFIX}")
    set(BROTLI_LIB_ENC "${CMAKE_SHARED_LIBRARY_PREFIX}brotlienc${CMAKE_SHARED_LIBRARY_SUFFIX}")
    set(BROTLI_LIB_DEC "${CMAKE_SHARED_LIBRARY_PREFIX}brotlidec${CMAKE_SHARED_LIBRARY_SUFFIX}")
    if (NOT BROTLI_PATH)
        find_path(BROTLI_PATH
            NAMES ${BROTLI_LIB_COM} ${BROTLI_LIB_ENC} ${BROTLI_LIB_DEC}
        )
    endif()
    if (NOT BROTLI_PATH)
        if (WIN32)
            find_path(BROTLI_PATH
                NAMES ${BROTLI_LIB_COM} ${BROTLI_LIB_ENC} ${BROTLI_LIB_DEC}
                # trying to find it from vcpkg
                HINTS "${VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/bin"
            )
        elseif(APPLE)
            message(FATAL_ERROR "adopt Qili to MacOS and delete this line")
        endif()
    else()
        find_path(BROTLI_PATH
            NAMES ${BROTLI_LIB_COM} ${BROTLI_LIB_ENC} ${BROTLI_LIB_DEC}
            HINTS "${BROTLI_PATH}" "${BROTLI_PATH}/bin" "${BROTLI_PATH}/lib64" "${BROTLI_PATH}/lib"
        )
    endif()

    if (NOT BROTLI_PATH)
        message(FATAL_ERROR "Please build with -DBROTLI_PATH=/path/to/brolti/libraries/ !")
    else()
        message(STATUS "Build with Brotli binaries from ${BROTLI_PATH}")
    endif()

    install(FILES
        ${BROTLI_PATH}/${BROTLI_LIB_COM}
        ${BROTLI_PATH}/${BROTLI_LIB_ENC}
        ${BROTLI_PATH}/${BROTLI_LIB_DEC}
        DESTINATION ${CMAKE_INSTALL_BINDIR}
    )
endif()

if (LINUX)
    configure_file(
        ${CMAKE_SOURCE_DIR}/IFW/Qili.linux
        ${CMAKE_BINARY_DIR}/Qili
        @ONLY)
    install(FILES
        ${CMAKE_BINARY_DIR}/Qili
        DESTINATION ${CMAKE_INSTALL_BINDIR}
        PERMISSIONS
            OWNER_READ OWNER_EXECUTE
            GROUP_READ GROUP_EXECUTE
            WORLD_READ WORLD_EXECUTE
    )
endif()

if (USE_IFW OR WIN32 OR APPLE)
    if (WIN32 OR APPLE OR (LINUX AND IFW_WITH_LIBS))
        qt_generate_deploy_script(
            TARGET QiliApp
            OUTPUT_SCRIPT deploy_script
            CONTENT [[
            set(QT_DEPLOY_TRANSLATIONS_DIR "${QILI_TRANSLATIONS_DIR}")
            qt_deploy_qt_conf(
                "\${QT_DEPLOY_PREFIX}/\${QT_DEPLOY_BIN_DIR}/qt.conf"
                PREFIX ..
                BIN_DIR ${CMAKE_INSTALL_BINDIR}
                LIB_DIR ${CMAKE_INSTALL_LIBDIR}
                PLUGINS_DIR plugins
                DATA_DIR ${CMAKE_INSTALL_DATADIR}/${QILI_PACKAGE_ID}
                TRANSLATIONS_DIR "${QILI_TRANSLATIONS_DIR}"
            )
            qt_deploy_translations(LOCALES en zh_CN zh_TW)
            message(STATUS "QT_DEPLOY_IGNORED_LIB_DIRS = ${QT_DEPLOY_IGNORED_LIB_DIRS}")
            qt_deploy_runtime_dependencies(
                EXECUTABLE $<TARGET_FILE:QiliApp>
                ADDITIONAL_LIBRARIES
                    $<TARGET_FILE:QiliApp>
                    $<TARGET_FILE:QiliWidgets>
                    $<TARGET_FILE:QRCodeGen>
                BIN_DIR ${CMAKE_INSTALL_BINDIR}
                LIB_DIR ${CMAKE_INSTALL_LIBDIR}
                POST_INCLUDE_REGEXES ".*brotli.*"
                NO_TRANSLATIONS
                VERBOSE
            )
            ]]
        )
        install(SCRIPT ${deploy_script})
    endif()
endif()
