cmake_minimum_required(VERSION 3.24)

project(system_explorer)

find_package(wxWidgets)

if(wxWidgets_USE_FILE)
        include(${wxWidgets_USE_FILE})
endif()

add_executable(system_explorer core/models/process.cpp  core/models/process_tree.cpp core/process_manager.cpp  gui/main_window.cpp app.cpp)
target_link_libraries(system_explorer ${wxWidgets_LIBRARIES})
