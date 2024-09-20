
include(NCPP/Utilities/ApplyDependencies)



# nrhi.samples.command_allocator target source files
file(GLOB source_files
    "source/*.cpp"
    "source/*.hpp"
)
source_group("source" FILES ${source_files})



# Create nrhi.samples.command_allocator target
add_executable(nrhi.samples.command_allocator
    ${source_files}
)



# nrhi.samples.command_allocator target settings
set_target_properties(nrhi.samples.command_allocator
    PROPERTIES 
        FOLDER "NCoder/nrhi/samples"
        PROJECT_LABEL "command_allocator"
)
target_link_libraries(nrhi.samples.command_allocator
    PUBLIC 
        nrhi
)
target_include_directories(nrhi.samples.command_allocator
    PUBLIC 
        nrhi
        ${CMAKE_CURRENT_SOURCE_DIR}/source
)



# nrhi.samples.command_allocator target dependencies
NCPP_ApplyDependencies(
    TARGET nrhi.samples.command_allocator
    DEPENDENT_TARGETS
        nrhi
)