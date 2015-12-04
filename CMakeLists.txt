if (NOT CMAKE_INSTALL_PREFIX)
 set (CMAKE_INSTALL_PREFIX /usr)
endif ()

cmake_minimum_required (VERSION 2.8.3)
include(CheckFunctionExists)

##
if ( NOT DEFAULT_ENCODING )
  set ( DEFAULT_ENCODING "UTF-8" )
endif ( NOT DEFAULT_ENCODING )

if ( NOT BUILD_QT_VERSION )
  set (BUILD_QT_VERSION 4)
endif ( NOT BUILD_QT_VERSION )

if ( BUILD_QT_VERSION STREQUAL 4 )
    set (QT4_BUILD 1)
    set (QT5_BUILD 0)
    set (QT_SUFF qt4)
else ( BUILD_QT_VERSION STREQUAL 4 )
    set (QT4_BUILD 0)
    set (QT5_BUILD 1)
    set (QT_SUFF qt5)
endif ( BUILD_QT_VERSION STREQUAL 4 )

message (STATUS "Build with QT version: ${BUILD_QT_VERSION}")
message (STATUS "QT4 build: ${QT4_BUILD}")
message (STATUS "QT5 build: ${QT5_BUILD}")
##

set (PROJECT_NAME se-sandbox-runner-${QT_SUFF})
set (PROJECT_VERSION 1.6.14)
configure_file ( mainwindow.cpp.in ${CMAKE_CURRENT_BINARY_DIR}/mainwindow.cpp )
set (HEADERS 
  mainwindow.h
  tray/traywidget.h
  toolbar/toolbar.h
  layout/listwidget.h
  layout/jobmenu.h
  layout/job_item_index.h
  layout/jobitem_model.h
  layout/progressbar_delegate.h
  element/elemprocess.h
  element/string_list.h
  element/shred_thread.h
  settings/settings.h
  settings/common_settings.h
  settings/window_settings.h
  settings/include_settings.h
  settings/directory_settings.h
  log/log_widget.h
  wait_thread.h)
set (SOURCES 
  main.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/mainwindow.cpp
  tray/traywidget.cpp
  toolbar/toolbar.cpp
  layout/listwidget.cpp
  layout/jobmenu.cpp
  layout/job_item_index.cpp
  layout/jobitem_model.cpp
  layout/progressbar_delegate.cpp
  element/elemprocess.cpp
  element/string_list.cpp
  element/shred_thread.cpp
  settings/settings.cpp
  settings/common_settings.cpp
  settings/window_settings.cpp
  settings/include_settings.cpp
  settings/directory_settings.cpp
  log/log_widget.cpp
  wait_thread.cpp)
set (SOURCES_DIRS . tray toolbar layout element settings)

project (${PROJECT_NAME})
include_directories (${CMAKE_CURRENT_BINARY_DIR} ${SOURCES_DIRS})
set (DESKTOP_FILE se-sandbox-runner.desktop)
set(RCC_SRC
    se_sandbox_runner.qrc
)
##
if ( QT4_BUILD )
  include ("CMakeQt4.txt")
else ( QT4_BUILD )
  include ("CMakeQt5.txt")
endif( QT4_BUILD )
##

add_executable (${PROJECT_NAME} ${HEADERS} ${SOURCES} ${MOC_SOURCES} ${RCC})
target_link_libraries (${PROJECT_NAME} ${BUILD_PROJECT_LIBRARIES})

configure_file ( ${DESKTOP_FILE}.in ${CMAKE_CURRENT_BINARY_DIR}/${DESKTOP_FILE} )
install ( FILES ${CMAKE_CURRENT_BINARY_DIR}/${DESKTOP_FILE}
    DESTINATION ${SHARE_INSTALL_PREFIX}/applications
    RENAME ${PROJECT_NAME}.desktop )
install (TARGETS ${PROJECT_NAME} DESTINATION bin)
