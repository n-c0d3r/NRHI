
include(NCPP/Utilities/ApplyDependencies)



# nrhi.samples.swapchain target source files
file(GLOB source_files
    "source/*.cpp"
    "source/*.hpp"
)
source_group("source" FILES ${source_files})



# Create nrhi.samples.swapchain target
add_executable(nrhi.samples.swapchain
    ${source_files}
)



# nrhi.samples.swapchain target settings
set_target_properties(nrhi.samples.swapchain
    PROPERTIES 
        FOLDER "NCoder/nrhi/samples"
        PROJECT_LABEL "swapchain"
)
target_link_libraries(nrhi.samples.swapchain
    PUBLIC 
        nrhi
)
target_include_directories(nrhi.samples.swapchain
    PUBLIC 
        nrhi
        ${CMAKE_CURRENT_SOURCE_DIR}/source
)



# nrhi.samples.swapchain target dependencies
NCPP_ApplyDependencies(
    TARGET nrhi.samples.swapchain
    DEPENDENT_TARGETS
        nrhi
)