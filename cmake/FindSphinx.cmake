include(FindPackageHandleStandardArgs)

message("FindSphinx.cmake EXECUTED")

# We are likely to find Sphinx near the Python interpreter
find_package(PythonInterp)
if(PYTHONINTERP_FOUND)
    get_filename_component(_PYTHON_DIR "${PYTHON_EXECUTABLE}" DIRECTORY)
    set(
        _PYTHON_PATHS
        "${_PYTHON_DIR}"
        "${_PYTHON_DIR}/bin"
        "${_PYTHON_DIR}/Scripts")
endif()

find_program(
    SPHINX_EXECUTABLE
    NAMES sphinx-build
    HINTS ${_PYTHON_PATHS}
	DOC "Path to the sphinx-build executable")

mark_as_advanced(SPHINX_EXECUTABLE)

find_package_handle_standard_args(Sphinx DEFAULT_MSG SPHINX_EXECUTABLE)
find_package_handle_standard_args(Sphinx "Unable to locate sphinx-build executable." SPHINX_EXECUTABLE)
