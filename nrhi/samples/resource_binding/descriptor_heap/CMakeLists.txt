
include(NCPP/Utilities/ApplyDependencies)



# nrhi.samples.resource_binding.descriptor_heap target source files
file(GLOB source_files
    "source/*.cpp"
    "source/*.hpp"
)
source_group("source" FILES ${source_files})



# Create nrhi.samples.resource_binding.descriptor_heap target
add_executable(nrhi.samples.resource_binding.descriptor_heap
    ${source_files}
)



# nrhi.samples.resource_binding.descriptor_heap target settings
set_target_properties(nrhi.samples.resource_binding.descriptor_heap
    PROPERTIES 
        FOLDER "NCoder/nrhi/samples"
        PROJECT_LABEL "resource_binding.descriptor_heap"
)
target_link_libraries(nrhi.samples.resource_binding.descriptor_heap
    PUBLIC 
        nrhi
)
target_include_directories(nrhi.samples.resource_binding.descriptor_heap
    PUBLIC 
        nrhi
        ${CMAKE_CURRENT_SOURCE_DIR}/source
)



# nrhi.samples.resource_binding.descriptor_heap target dependencies
NCPP_ApplyDependencies(
    TARGET nrhi.samples.resource_binding.descriptor_heap
    DEPENDENT_TARGETS
        nrhi
)