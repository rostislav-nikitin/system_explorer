
find_package(Doxygen REQUIRED)
if(NOT DOXYGEN_FOUND)
	return()
endif()

set(DOXYGEN_GENERATE_HTML NO)
set(DOXYGEN_GENERATE_XML YES)

set(DOXYGEN_STRIP_FROM_PATH ${CMAKE_SOURCE_DIR})
set(DOXYGEN_EXCLUDE_PATTERNS "*.cpp")


doxygen_add_docs(doxygen-doc ${CMAKE_CURRENT_SOURCE_DIR}/../core ${CMAKE_CURRENT_SOURCE_DIR}/../gui  COMMENT "Generate APP core documentation with Doxygen")

find_package(Sphinx REQUIRED)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/conf.py.in
	${CMAKE_CURRENT_BINARY_DIR}/conf.py @ONLY)

add_custom_target(
	sphinx-doc ALL
	COMMAND ${SPHINX_EXECUTABLE} -b html -c ${CMAKE_CURRENT_BINARY_DIR}
	${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR}
	WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
	COMMENT "Generation APP documentation with Sphinx"
	VERBATIM)

add_dependencies(sphinx-doc doxygen-doc)
