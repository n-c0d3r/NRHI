
include(NCPP/Utilities/ApplyDependencies)



# nrhi.samples.resource_binding.descriptor target source files
file(GLOB source_files
    "source/*.cpp"
    "source/*.hpp"
)
source_group("source" FILES ${source_files})



# Create nrhi.samples.resource_binding.descriptor target
add_executable(nrhi.samples.resource_binding.descriptor
    ${source_files}
)



# nrhi.samples.resource_binding.descriptor target settings
set_target_properties(nrhi.samples.resource_binding.descriptor
    PROPERTIES 
        FOLDER "NCoder/nrhi/samples"
        PROJECT_LABEL "resource_binding.descriptor"
)
target_link_libraries(nrhi.samples.resource_binding.descriptor
    PUBLIC 
        nrhi
)
target_include_directories(nrhi.samples.resource_binding.descriptor
    PUBLIC 
        nrhi
        ${CMAKE_CURRENT_SOURCE_DIR}/source
)



# nrhi.samples.resource_binding.descriptor target dependencies
NCPP_ApplyDependencies(
    TARGET nrhi.samples.resource_binding.descriptor
    DEPENDENT_TARGETS
        nrhi
)