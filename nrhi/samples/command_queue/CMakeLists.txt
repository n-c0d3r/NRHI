
include(NCPP/Utilities/ApplyDependencies)



# nrhi.samples.command_queue target source files
file(GLOB source_files
    "source/*.cpp"
    "source/*.hpp"
)
source_group("source" FILES ${source_files})



# Create nrhi.samples.command_queue target
add_executable(nrhi.samples.command_queue
    ${source_files}
)



# nrhi.samples.command_queue target settings
set_target_properties(nrhi.samples.command_queue
    PROPERTIES 
        FOLDER "NCoder/nrhi/samples"
        PROJECT_LABEL "command_queue"
)
target_link_libraries(nrhi.samples.command_queue
    PUBLIC 
        nrhi
)
target_include_directories(nrhi.samples.command_queue
    PUBLIC 
        nrhi
        ${CMAKE_CURRENT_SOURCE_DIR}/source
)



# nrhi.samples.command_queue target dependencies
NCPP_ApplyDependencies(
    TARGET nrhi.samples.command_queue
    DEPENDENT_TARGETS
        nrhi
)