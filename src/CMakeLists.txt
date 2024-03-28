cmake_minimum_required(VERSION 3.24)

#project("System Explorer" VERSION 0.0.1 LANGUAGES CXX)
#project(system_explorer)

# wxWidgest
find_package(wxWidgets)

if(wxWidgets_USE_FILE)
        include(${wxWidgets_USE_FILE})
endif()

# google test
include(FetchContent)

FetchContent_Declare(googletest
	GIT_REPOSITORY https://github.com/google/googletest
	GIT_TAG v1.14.0
)

FetchContent_GetProperties(googletest)

if(NOT FetchContent_Populated)
	FetchContent_Populate(googletest)
	#add_subdirectory(${googletest_SOURCE_DIR} ${googletest_BUILD_DIR})
	add_subdirectory(${googletest_SOURCE_DIR} googletest_build)
	message("${googletest_SOURCE_DIR}::${googletest_BUILD_DIR}")
endif()

#Resources
add_custom_target(BuildResourcesIcons ALLDEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/resources/resources.icons.cpp)
add_custom_command (OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/resources/resources.icons.cpp
                     COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/build_icons_resouces.sh
                     WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/)

#Libs
include_directories(libs/wild_card_expr/include)
add_library(wild_card_expr libs/wild_card_expr/src/wild_card_expr.cpp)

include_directories(libs/fs/include)
add_library(fs libs/fs/src/fs.cpp)

#App
add_executable(system_explorer 
	resources/resources.icons.cpp

	config/config.cpp
	config/user_config.cpp

	
	core/models/signal_type.cpp 
	core/models/signal.cpp  
	core/signal_types_dict.cpp 
	core/signals_dict.cpp 
	core/signal_manager.cpp 

	core/models/process_state_helpers.cpp
	core/models/process.cpp
	core/models/process_tree.cpp 
	core/process_manager.cpp

	core/proc_tree_stat_manager.cpp
	core/processes_stat_manager.cpp
	
	core/parsers/proc_stat_parser.cpp
	core/parsers/proc_process_stat_parser.cpp

	gui/control/control_base.cpp
	gui/control/searchable_control_base.cpp
	gui/control/searchable_treelist_control_base.cpp
	gui/control/icon_control.cpp
	gui/control/searchable_list_control.cpp
	gui/control/searchable_treelist_control.cpp
	gui/control/searchable_listview_control.cpp

	gui/view_controller/view_controller_base.cpp
	gui/view_controller/root_view_controller.cpp
	gui/view_controller/about_dialog_view_controller.cpp
	gui/view_controller/processes_tree_list_item_comparator.cpp 
	gui/view_controller/process_tree_view_controller.cpp

	app.cpp)

target_link_libraries(system_explorer wild_card_expr fs ${wxWidgets_LIBRARIES})

# Tests

#add_executable(process_manager_test_runner 
#	core/process_manager.cpp
#	tests/core/process_manager_test_runner.cpp)
#target_link_libraries(process_manager_test_runner gmock gtest ${googletest_LIBRARIES})

add_executable(proc_stat_parser_test_runner 
	core/parsers/proc_stat_parser.cpp
	tests/core/proc_stat_parser_test_runner.cpp)

target_link_libraries(proc_stat_parser_test_runner gmock gtest ${googletest_LIBRARIES})

add_executable(proc_process_stat_parser_test_runner 
	core/parsers/proc_process_stat_parser.cpp
	tests/core/proc_process_stat_parser_test_runner.cpp)

target_link_libraries(proc_process_stat_parser_test_runner gmock gtest ${googletest_LIBRARIES})

#file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/assets DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
           
#list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")
#message("${CMAKE_MODULE_PATH}")

add_subdirectory(doc)
