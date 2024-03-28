
#####################################################################################
#   Includes
#####################################################################################
include(NCPP/ConfigureCompiler)



#####################################################################################
#   Generated files
#####################################################################################
# nrhi target configure file
configure_file(source/nrhi/.config "${NRHI_GENERATED_FILES_DIR}/config.hpp")



#####################################################################################
#   nrhi target's files
#####################################################################################
set(NRHI_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/source" PARENT_SCOPE)

# nrhi target source files
file(GLOB_RECURSE source_files
    "source/*.cpp"
    "source/*.hpp"

    "source/*.mm"
    "source/*.h"
)

# Exclude the windows platform files from the list if not using windows
if(NOT WIN32)
    list(FILTER source_files EXCLUDE REGEX "source/nrhi/windows/*.*")
endif()

# Exclude the apple platform files from the list if not using apple platforms
if(NOT APPLE)
    list(FILTER source_files EXCLUDE REGEX "source/nrhi/macos/*.*")
endif()

# Exclude the posix platform files from the list if not using posix
if(NOT UNIX)
    list(FILTER source_files EXCLUDE REGEX "source/nrhi/posix/*.*")
endif()

# create source group
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/source PREFIX source FILES ${source_files})

file(GLOB_RECURSE generated_files
    "${CMAKE_CURRENT_BINARY_DIR}/generated_files/*.cpp"
    "${CMAKE_CURRENT_BINARY_DIR}/generated_files/*.hpp"
)
source_group(TREE "${CMAKE_CURRENT_BINARY_DIR}/generated_files" PREFIX generated_files FILES ${generated_files})



#####################################################################################
#   Create nrhi target
#####################################################################################
add_library(nrhi STATIC
    ${source_files}
    ${generated_files}
)



#####################################################################################
#   nrhi target settings
#####################################################################################
set_target_properties(nrhi
    PROPERTIES 
        FOLDER "nrhi"
)

target_precompile_headers(nrhi
    PRIVATE
        source/nrhi/prerequisites.hpp
)

target_include_directories(nrhi
    PUBLIC 
        ${CMAKE_CURRENT_SOURCE_DIR}/source
        ${CMAKE_CURRENT_BINARY_DIR}/generated_files
)

target_link_libraries(nrhi
    PUBLIC
        ncpp
        nmath
        nsurface
)

# MSVC compile options
if(MSVC)
  target_compile_options(nrhi PUBLIC /GT)
endif()

# MacOS libs
if(APPLE)
    # Find the Cocoa lib
    find_library(COCOA_LIBRARY Cocoa)
    if (NOT COCOA_LIBRARY)
        message(FATAL_ERROR "Cocoa not found")
    endif()
 
    target_link_libraries(nrhi
        PRIVATE
            ${COCOA_LIBRARY}
    )
endif()



#####################################################################################
#   Dependencies
#####################################################################################
add_dependencies(nrhi
    ncpp
    nmath
)



#####################################################################################
#   Add dependencies
#####################################################################################
add_subdirectory(samples)



