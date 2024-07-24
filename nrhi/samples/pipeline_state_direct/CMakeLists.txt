
# nrhi.samples.pipeline_state_direct target source files
file(GLOB source_files
    "source/*.cpp"
    "source/*.hpp"
)
source_group("source" FILES ${source_files})



# Create nrhi.samples.pipeline_state_direct target
add_executable(nrhi.samples.pipeline_state_direct
    ${source_files}
)



# nrhi.samples.pipeline_state_direct target settings
set_target_properties(nrhi.samples.pipeline_state_direct
    PROPERTIES 
        FOLDER "NCoder/nrhi/samples"
        PROJECT_LABEL "pipeline_state_direct"
)
target_link_libraries(nrhi.samples.pipeline_state_direct
    PUBLIC 
        nrhi
)
target_include_directories(nrhi.samples.pipeline_state_direct
    PUBLIC 
        nrhi
        ${CMAKE_CURRENT_SOURCE_DIR}/source
)



# nrhi.samples.pipeline_state_direct target dependencies
add_dependencies(nrhi.samples.pipeline_state_direct 
    nrhi
)