
#####################################################################################
#   Includes
#####################################################################################
include(NCPP/ConfigureCompiler)
include(NCPP/Utilities/SetGlobal)



#####################################################################################
#   Options
#####################################################################################
# NRHI linking options
option(NRHI_DLL "Build NRHI as DLL or not" OFF)

# NRHI driver options
option(NRHI_DRIVER_DIRECTX_12 "If checked, NRHI will use Directx12 driver" OFF)
option(NRHI_DRIVER_DIRECTX_11 "If checked, NRHI will use Directx11 driver" ON)
option(NRHI_DRIVER_VULKAN "If checked, NRHI will use Vulkan driver" OFF)
option(NRHI_DRIVER_METAL "If checked, NRHI will use Metal driver" OFF)
option(NRHI_DRIVER_OPENGL "If checked, NRHI will use OpenGL driver" OFF)

# NRHI platform limit options
set(NRHI_MAX_RTV_COUNT_PER_DRAWCALL 8 CACHE STRING "Max render target view count per drawcall")
set(NRHI_MAX_VERTEX_BUFFER_COUNT_PER_DRAWCALL 16 CACHE STRING "Max vertex buffer count per drawcall")
set(NRHI_MAX_INSTANCE_BUFFER_COUNT_PER_DRAWCALL 16 CACHE STRING "Max instance buffer count per drawcall")
set(NRHI_MAX_VERTEX_INSTANCE_BUFFER_COUNT_PER_DRAWCALL 16 CACHE STRING "On some platforms, there is a limit of the combination count of vertex buffers and instance buffers")
set(NRHI_MAX_CONSTANT_BUFFER_COUNT_PER_DRAWCALL 16 CACHE STRING "Max constant buffer count per drawcall")

# NRHI threading options
option(NRHI_THREAD_SAFE "If checked, NRHI will use locks when creating and releasing objects" OFF)

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
    NCPP_SetGlobal(NRHI_ENABLE_DXGI ON)
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

# Exclude the directx11 platform files from the list if not using directx11
if(NOT NRHI_DRIVER_DIRECTX_11)
    list(FILTER source_files EXCLUDE REGEX "source/nrhi/directx11/*.*")
endif()

# Exclude the directx12 platform files from the list if not using directx12
if(NOT NRHI_DRIVER_DIRECTX_12)
    list(FILTER source_files EXCLUDE REGEX "source/nrhi/directx12/*.*")
endif()

# Exclude the dxgi platform files from the list if not using dxgi
if(NOT NRHI_ENABLE_DXGI)
    list(FILTER source_files EXCLUDE REGEX "source/nrhi/dxgi/*.*")
endif()

# Exclude the vulkan platform files from the list if not using vulkan
if(NOT NRHI_DRIVER_VULKAN)
    list(FILTER source_files EXCLUDE REGEX "source/nrhi/vulkan/*.*")
endif()

# Exclude the metal platform files from the list if not using metal
if(NOT NRHI_DRIVER_METAL)
    list(FILTER source_files EXCLUDE REGEX "source/nrhi/metal/*.*")
endif()

# Exclude the opengl platform files from the list if not using opengl
if(NOT NRHI_DRIVER_OPENGL)
    list(FILTER source_files EXCLUDE REGEX "source/nrhi/opengl/*.*")
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
if(${NRHI_DLL})
    set(nrhiLinkingMode "SHARED")
else()
    set(nrhiLinkingMode "STATIC")
endif()
add_library(nrhi ${nrhiLinkingMode}
    ${source_files}
    ${generated_files}
)



#####################################################################################
#   nrhi target settings
#####################################################################################
set_target_properties(nrhi
    PROPERTIES 
        FOLDER "NCoder/nrhi"
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

if(NRHI_DRIVER_DIRECTX_12)
    target_link_libraries(nrhi
        PUBLIC
            d3d12.lib
            d3dcompiler.lib
    )
endif()
if(NRHI_DRIVER_DIRECTX_11)
    target_link_libraries(nrhi
        PUBLIC
            d3d11.lib
            d3dcompiler.lib
    )
endif()
if(NRHI_ENABLE_DXGI)
    target_link_libraries(nrhi
        PUBLIC
            dxgi.lib
    )
endif()

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

if(${NRHI_DLL})
    target_compile_definitions(nrhi
        PRIVATE
            -DNRHI_DLL_IMPLEMENTATION
    )
endif()



#####################################################################################
#   Dependencies
#####################################################################################
add_dependencies(nrhi
    ncpp
    nmath
    nsurface
)



#####################################################################################
#   Add dependencies
#####################################################################################
add_subdirectory(samples)



