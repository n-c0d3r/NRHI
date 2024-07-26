
# nrhi.samples.pipeline_state_with_root_signature target source files
file(GLOB source_files
    "source/*.cpp"
    "source/*.hpp"
)
source_group("source" FILES ${source_files})



# Create nrhi.samples.pipeline_state_with_root_signature target
add_executable(nrhi.samples.pipeline_state_with_root_signature
    ${source_files}
)



# nrhi.samples.pipeline_state_with_root_signature target settings
set_target_properties(nrhi.samples.pipeline_state_with_root_signature
    PROPERTIES 
        FOLDER "NCoder/nrhi/samples"
        PROJECT_LABEL "pipeline_state_with_root_signature"
)
target_link_libraries(nrhi.samples.pipeline_state_with_root_signature
    PUBLIC 
        nrhi
)
target_include_directories(nrhi.samples.pipeline_state_with_root_signature
    PUBLIC 
        nrhi
        ${CMAKE_CURRENT_SOURCE_DIR}/source
)



# nrhi.samples.pipeline_state_with_root_signature target dependencies
add_dependencies(nrhi.samples.pipeline_state_with_root_signature 
    nrhi
)