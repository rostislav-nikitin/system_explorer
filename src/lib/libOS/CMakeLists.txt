 set(OS_INCLUDE_DIRS include PARENT_SCOPE)

add_library(libOS 
    src/process/model/process.cpp
    src/process/process_manager.cpp)

target_include_directories(libOS PRIVATE ../libWCE/include)
target_link_libraries(libOS libWCE)

#add_subdirectory(doc)