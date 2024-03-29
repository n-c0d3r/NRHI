
#####################################################################################
#   Includes
#####################################################################################
include(NCPP/ConfigureCompiler)



#####################################################################################
#   Options
#####################################################################################
# NRHI variable dynamic linking options
option(NRHI_VARIABLE_DYNAMIC_LINKING "If checked, NRHI will use pointers for some indicated dynamic linked global variables" ON)

# NRHI driver options
option(NRHI_DRIVER_DIRECTX_12 "If checked, NRHI will use Directx12 driver" OFF)
option(NRHI_DRIVER_DIRECTX_11 "If checked, NRHI will use Directx11 driver" ON)
option(NRHI_DRIVER_VULKAN "If checked, NRHI will use Vulkan driver" OFF)
option(NRHI_DRIVER_METAL "If checked, NRHI will use Metal driver" OFF)
option(NRHI_DRIVER_OPENGL "If checked, NRHI will use OpenGL driver" OFF)

# Compute driver count
set(NRHI_DRIVER_COUNT 0)
if(NRHI_DRIVER_DIRECTX_12)
    NCPP_SetGlobal(NRHI_DRIVER_INDEX_DIRECTX_12 ${NRHI_DRIVER_COUNT})
    math(EXPR NRHI_DRIVER_COUNT "${NRHI_DRIVER_COUNT} + 1")
endif()
if(NRHI_DRIVER_DIRECTX_11)
    NCPP_SetGlobal(NRHI_DRIVER_INDEX_DIRECTX_11 ${NRHI_DRIVER_COUNT})
    math(EXPR NRHI_DRIVER_COUNT "${NRHI_DRIVER_COUNT} + 1")
endif()
if(NRHI_DRIVER_VULKAN)
    NCPP_SetGlobal(NRHI_DRIVER_INDEX_VULKAN ${NRHI_DRIVER_COUNT})
    math(EXPR NRHI_DRIVER_COUNT "${NRHI_DRIVER_COUNT} + 1")
endif()
if(NRHI_DRIVER_METAL)
    NCPP_SetGlobal(NRHI_DRIVER_INDEX_METAL ${NRHI_DRIVER_COUNT})
    math(EXPR NRHI_DRIVER_COUNT "${NRHI_DRIVER_COUNT} + 1")
endif()
if(NRHI_DRIVER_OPENGL)
    NCPP_SetGlobal(NRHI_DRIVER_INDEX_OPENGL ${NRHI_DRIVER_COUNT})
    math(EXPR NRHI_DRIVER_COUNT "${NRHI_DRIVER_COUNT} + 1")
endif()
NCPP_ApplyGlobal(NRHI_DRIVER_COUNT)

set(NRHI_DRIVER_LAST_INDEX 0)
math(EXPR NRHI_DRIVER_LAST_INDEX "${NRHI_DRIVER_COUNT} - 1")
NCPP_ApplyGlobal(NRHI_DRIVER_LAST_INDEX)

# For abstraction solution (run-time/compile-time driver chosen)
if(NRHI_DRIVER_COUNT EQUAL 1)
    NCPP_SetGlobal(NRHI_DRIVER_SINGLE ON)
    NCPP_SetGlobal(NRHI_DRIVER_MULTIPLE OFF)
else()
    NCPP_SetGlobal(NRHI_DRIVER_MULTIPLE ON)
    NCPP_SetGlobal(NRHI_DRIVER_SINGLE OFF)
endif()

if(NOT NRHI_DRIVER_COUNT)
    message(FATAL_ERROR "No driver enabled")
endif()

# Implement drivers
if(NRHI_DRIVER_DIRECTX_11 OR NRHI_DRIVER_DIRECTX_12)
    include(NRHI/Drivers/DXGI/Implement)
endif()
if(NRHI_DRIVER_DIRECTX_12)
    include(NRHI/Drivers/DirectX12/Implement)
endif()
if(NRHI_DRIVER_DIRECTX_11)
    include(NRHI/Drivers/DirectX11/Implement)
endif()
if(NRHI_DRIVER_VULKAN)
    include(NRHI/Drivers/Vulkan/Implement)
endif()
if(NRHI_DRIVER_METAL)
    include(NRHI/Drivers/Metal/Implement)
endif()
if(NRHI_DRIVER_OPENGL)
    include(NRHI/Drivers/OpenGL/Implement)
endif()



#####################################################################################
#   Generated files
#####################################################################################
# NRHI target configure file
configure_file(source/nrhi/.config "${NRHI_GENERATED_FILES_DIR}/config.hpp")



#####################################################################################
#   NRHI target's files
#####################################################################################
set(NRHI_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/source" PARENT_SCOPE)

# NRHI target source files
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

# Create source group
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/source PREFIX source FILES ${source_files})

file(GLOB_RECURSE generated_files
    "${CMAKE_CURRENT_BINARY_DIR}/generated_files/*.cpp"
    "${CMAKE_CURRENT_BINARY_DIR}/generated_files/*.hpp"
)
source_group(TREE "${CMAKE_CURRENT_BINARY_DIR}/generated_files" PREFIX generated_files FILES ${generated_files})



#####################################################################################
# Create nrhi target
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



