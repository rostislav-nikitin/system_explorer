cmake_minimum_required(VERSION 3.14)

project("System Explorer" VERSION 0.0.1 LANGUAGES CXX)
            
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")
message("${CMAKE_MODULE_PATH}")


add_subdirectory(src)
#add_subdirectory(doc)
