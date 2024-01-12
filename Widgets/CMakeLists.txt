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
# Resuable widgets for Qili      #
##################################

set(WIDGETS_TS_FILES QiliWidgets_zh_CN.ts)
set(WIDGETS_RC_FILES QiliWidgets.qrc)
set(WIDGETS_SRC_FILES
    WidgetsGlobal.h

    QiliDialog.h
    QiliDialog.cpp

    QiliTextField.h
    QiliTextField.cpp

    QiliTitleBar.h
    QiliTitleBar.cpp
    QiliTitleBar.ui
)

qili_add_library(QiliWidgets)
target_compile_definitions(QiliWidgets PRIVATE QILI_WIDGETS_LIBRARY)
target_include_directories(QiliWidgets PRIVATE ${CMAKE_BINARY_DIR})
target_sources(QiliWidgets PRIVATE
    ${WIDGETS_SRC_FILES}
    ${WIDGETS_TS_FILES}
    ${WIDGETS_RC_FILES}
)
target_link_libraries(QiliWidgets PRIVATE Qt::Core Qt::Widgets)

if (QT_VERSION_MAJOR EQUAL 5)
    qt5_create_translation(WIDGETS_QM_FILES ${WIDGETS_SRC_FILES} ${WIDGETS_TS_FILES})
    add_custom_target(QiliWidgets_lrelease ALL DEPENDS ${WIDGETS_QM_FILES})
else()
    qt_add_translations(QiliWidgets TS_FILES ${WIDGETS_TS_FILES} QM_FILES_OUTPUT_VARIABLE WIDGETS_QM_FILES)
endif()


install(TARGETS QiliWidgets
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    COMPONENT Runtime
)
install(FILES ${WIDGETS_QM_FILES} DESTINATION ${CMAKE_INSTALL_DATADIR}/translations)

qili_finalize_target(QiliWidgets)
