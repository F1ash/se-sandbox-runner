cmake_minimum_required (VERSION 2.8.3)

set (PROJECT_NAME se-sandbox-runner)
set (PROJECT_VERSION 1.0)
set (HEADERS 
  mainwindow.h
  tray/traywidget.h
  toolbar/toolbar.h
  layout/listwidget.h
  layout/jobmenu.h
  #utils/pgrep.h
  element/elemprocess.h
  settings/settings.h
  settings/common_settings.h
  settings/window_settings.h
  settings/include_settings.h
  settings/directory_settings.h
  element/string_list.h
  wait_thread.h)
set (SOURCES 
  main.cpp
  mainwindow.cpp
  tray/traywidget.cpp
  toolbar/toolbar.cpp
  layout/listwidget.cpp
  layout/jobmenu.cpp
  #utils/pgrep.cpp
  element/elemprocess.cpp
  settings/settings.cpp
  settings/common_settings.cpp
  settings/window_settings.cpp
  settings/include_settings.cpp
  settings/directory_settings.cpp
  element/string_list.cpp
  wait_thread.cpp)
set (DOC_FILES README.md COPYING Changelog icons/Licenses)
set (DESKTOP_FILE ${PROJECT_NAME}.desktop)
set (ICON_SIZES 256x256)

project (${PROJECT_NAME})

if (NOT CMAKE_INSTALL_PREFIX) 
 set (CMAKE_INSTALL_PREFIX /usr) 
endif ()

include_directories (${CMAKE_CURRENT_BINARY_DIR} . tray toolbar layout element settings)
find_package (Qt4 COMPONENTS QtCore QtGui REQUIRED)
include (${QT_USE_FILE})
qt4_wrap_cpp (MOC_SOURCES ${HEADERS})
set (BUILD_PROJECT_LIBRARIES ${QT_LIBRARIES})

source_group ("Header Files" FILES ${HEADERS})
source_group ("Source Files" FILES ${SOURCES})
source_group ("Generated Files" FILES ${MOC_SOURCES})

add_executable (${PROJECT_NAME} ${HEADERS} ${SOURCES} ${MOC_SOURCES})
target_link_libraries (${PROJECT_NAME} ${BUILD_PROJECT_LIBRARIES})

install (TARGETS ${PROJECT_NAME} DESTINATION bin)
install(FILES ${DESKTOP_FILE} DESTINATION share/applications)

install(FILES icons/applications-safety.png 
        DESTINATION share/icons/hicolor/${ICON_SIZE}/apps)

