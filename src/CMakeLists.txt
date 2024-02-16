cmake_minimum_required(VERSION 3.24)

project(system_explorer)

find_package(wxWidgets)

if(wxWidgets_USE_FILE)
        include(${wxWidgets_USE_FILE})
endif()

add_executable(system_explorer 
	core/models/signal_type.cpp core/models/signal.cpp  
	core/signal_types_dict.cpp core/signals_dict.cpp core/signal_manager.cpp 
	core/models/process.cpp core/models/process_tree.cpp 
	core/process_manager.cpp 
	gui/control/searchable_treelist_control.cpp
	gui/process_data.cpp gui/processes_tree_list_item_comparator.cpp gui/main_window.cpp app.cpp)

target_link_libraries(system_explorer ${wxWidgets_LIBRARIES})
