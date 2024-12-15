include_directories(${CMAKE_CURRENT_LIST_DIR})

if (NOT DEFINED XPROCESS_SOURCES)
    include(${CMAKE_CURRENT_LIST_DIR}/../XProcess/xprocess.cmake)
    set(XPROCESSMEMORYMAPWIDGET_SOURCES ${XPROCESSMEMORYMAPWIDGET_SOURCES} ${XPROCESS_SOURCES})
endif()

set(XPROCESSMEMORYMAPWIDGET_SOURCES
    ${XPROCESSMEMORYMAPWIDGET_SOURCES}
    ${XPROCESS_SOURCES}
    ${CMAKE_CURRENT_LIST_DIR}/dialogxprocessmemorymap.cpp
    ${CMAKE_CURRENT_LIST_DIR}/dialogxprocessmemorymap.h
    ${CMAKE_CURRENT_LIST_DIR}/dialogxprocessmemorymap.ui
    ${CMAKE_CURRENT_LIST_DIR}/xprocessmemorymapwidget.cpp
    ${CMAKE_CURRENT_LIST_DIR}/xprocessmemorymapwidget.h
    ${CMAKE_CURRENT_LIST_DIR}/xprocessmemorymapwidget.ui
)
