 set(OS_INCLUDE_DIRS include PARENT_SCOPE)

add_library(libOS 
    src/process/model/process.cpp
    src/process/process_manager.cpp
    
    src/signal/model/signal_type.cpp
    src/signal/model/signal.cpp
    src/signal/signal_types_dict.cpp
    src/signal/signals_dict.cpp
    src/signal/signal_manager.cpp

    src/stat/parser/proc_process_stat_parser.cpp
    src/stat/parser/proc_stat_parser.cpp
    src/stat/process_state_helpers.cpp
    src/stat/proc_tree_stat_manager.cpp
    src/stat/processes_stat_manager.cpp
)

target_include_directories(libOS PRIVATE ../libWCE/include)
target_link_libraries(libOS libWCE)

add_subdirectory(doc)
