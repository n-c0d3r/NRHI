
# nrhi.samples.swapchain_with_clear target source files
file(GLOB source_files
    "source/*.cpp"
    "source/*.hpp"
)
source_group("source" FILES ${source_files})



# Create nrhi.samples.swapchain_with_clear target
add_executable(nrhi.samples.swapchain_with_clear
    ${source_files}
)



# nrhi.samples.swapchain_with_clear target settings
set_target_properties(nrhi.samples.swapchain_with_clear
    PROPERTIES 
        FOLDER "NCoder/nrhi/samples"
        PROJECT_LABEL "swapchain_with_clear"
)
target_link_libraries(nrhi.samples.swapchain_with_clear
    PUBLIC 
        nrhi
)
target_include_directories(nrhi.samples.swapchain_with_clear
    PUBLIC 
        nrhi
        ${CMAKE_CURRENT_SOURCE_DIR}/source
)



# nrhi.samples.swapchain_with_clear target dependencies
add_dependencies(nrhi.samples.swapchain_with_clear 
    nrhi
)