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


############################################
# Qili widgets plugin for Qt 6 Designer    #
############################################

set(DESIGNER_TS_FILES QiliDesigner_zh_CN.ts)

qili_add_plugin(QiliDesigner)
target_compile_definitions(QiliDesigner PRIVATE QILI_DESIGNER_LIBRARY)

target_sources(QiliDesigner PRIVATE
    DesignerGlobal.h
    QiliDesigner.h QiliDesigner.cpp
    QiliTitleBarDesigner.h QiliTitleBarDesigner.cpp
    QiliTextFieldDesigner.h QiliTextFieldDesigner.cpp
    ${DESIGNER_RC_FILES}
)
target_link_libraries(QiliDesigner PRIVATE
    Qt::Core
    Qt::Gui
    Qt::UiPlugin
    Qt::Widgets
    $<TARGET_LINKER_FILE:QiliWidgets>
)
add_dependencies(QiliDesigner QiliWidgets)
target_include_directories(QiliDesigner PUBLIC ${PROJECT_SOURCE_DIR}/Widgets)

set(QDESIGNER_PLUGINS_DIR plugins/designer)

install(TARGETS QiliDesigner
    BUNDLE DESTINATION  ${QDESIGNER_PLUGINS_DIR}
    LIBRARY DESTINATION ${QDESIGNER_PLUGINS_DIR}
    RUNTIME DESTINATION ${QDESIGNER_PLUGINS_DIR}
    COMPONENT Development
)
qili_finalize_target(QiliDesigner)
