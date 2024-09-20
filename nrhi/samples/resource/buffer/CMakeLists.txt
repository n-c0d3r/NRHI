
include(NCPP/Utilities/ApplyDependencies)



# nrhi.samples.resource.buffer target source files
file(GLOB source_files
    "source/*.cpp"
    "source/*.hpp"
)
source_group("source" FILES ${source_files})



# Create nrhi.samples.resource.buffer target
add_executable(nrhi.samples.resource.buffer
    ${source_files}
)



# nrhi.samples.resource.buffer target settings
set_target_properties(nrhi.samples.resource.buffer
    PROPERTIES 
        FOLDER "NCoder/nrhi/samples"
        PROJECT_LABEL "resource.buffer"
)
target_link_libraries(nrhi.samples.resource.buffer
    PUBLIC 
        nrhi
)
target_include_directories(nrhi.samples.resource.buffer
    PUBLIC 
        nrhi
        ${CMAKE_CURRENT_SOURCE_DIR}/source
)



# nrhi.samples.resource.buffer target dependencies
NCPP_ApplyDependencies(
    TARGET nrhi.samples.resource.buffer
    DEPENDENT_TARGETS
        nrhi
)