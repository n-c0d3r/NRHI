
# nrhi.samples.swapchain_present_alternative target source files
file(GLOB source_files
    "source/*.cpp"
    "source/*.hpp"
)
source_group("source" FILES ${source_files})



# Create nrhi.samples.swapchain_present_alternative target
add_executable(nrhi.samples.swapchain_present_alternative
    ${source_files}
)



# nrhi.samples.swapchain_present_alternative target settings
set_target_properties(nrhi.samples.swapchain_present_alternative
    PROPERTIES 
        FOLDER "NCoder/nrhi/samples"
        PROJECT_LABEL "swapchain_present_alternative"
)
target_link_libraries(nrhi.samples.swapchain_present_alternative
    PUBLIC
        nrhi.with_interface_only_supports
)
target_include_directories(nrhi.samples.swapchain_present_alternative
    PUBLIC
        nrhi.with_interface_only_supports
        ${CMAKE_CURRENT_SOURCE_DIR}/source
)



# nrhi.samples.swapchain_present_alternative target dependencies
add_dependencies(nrhi.samples.swapchain_present_alternative
    nrhi.with_interface_only_supports
)