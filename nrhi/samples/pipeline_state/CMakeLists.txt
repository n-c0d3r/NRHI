
include(NCPP/Utilities/ApplyDependencies)



# nrhi.samples.pipeline_state target source files
file(GLOB source_files
    "source/*.cpp"
    "source/*.hpp"
)
source_group("source" FILES ${source_files})



# Create nrhi.samples.pipeline_state target
add_executable(nrhi.samples.pipeline_state
    ${source_files}
)



# nrhi.samples.pipeline_state target settings
set_target_properties(nrhi.samples.pipeline_state
    PROPERTIES 
        FOLDER "NCoder/nrhi/samples"
        PROJECT_LABEL "pipeline_state"
)
target_link_libraries(nrhi.samples.pipeline_state
    PUBLIC 
        nrhi
)
target_include_directories(nrhi.samples.pipeline_state
    PUBLIC 
        nrhi
        ${CMAKE_CURRENT_SOURCE_DIR}/source
)



# nrhi.samples.pipeline_state target dependencies
NCPP_ApplyDependencies(
    TARGET nrhi.samples.pipeline_state
    DEPENDENT_TARGETS
        nrhi
)