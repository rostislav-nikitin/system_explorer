# wxWidgest
find_package(wxWidgets)

if(wxWidgets_USE_FILE)
        include(${wxWidgets_USE_FILE})
endif()


set(UI_INCLUDE_DIRS include PARENT_SCOPE)

add_library(libUI
    src/ui/control/control_base.cpp
    src/ui/control/icon_control.cpp
    src/ui/control/searchable_control_base.cpp
    src/ui/control/searchable_list_control.cpp
    src/ui/control/searchable_treelist_control_base.cpp
    src/ui/control/searchable_treelist_control.cpp
    src/ui/control/searchable_listview_control.cpp
)

target_include_directories(libUI PRIVATE ../libWCE/include)
target_link_libraries(libOS libWCE ${wxWidgets_LIBRARIES})

add_subdirectory(doc)