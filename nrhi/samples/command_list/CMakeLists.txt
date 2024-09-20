
include(NCPP/Utilities/ApplyDependencies)



# nrhi.samples.command_list target source files
file(GLOB source_files
    "source/*.cpp"
    "source/*.hpp"
)
source_group("source" FILES ${source_files})



# Create nrhi.samples.command_list target
add_executable(nrhi.samples.command_list
    ${source_files}
)



# nrhi.samples.command_list target settings
set_target_properties(nrhi.samples.command_list
    PROPERTIES 
        FOLDER "NCoder/nrhi/samples"
        PROJECT_LABEL "command_list"
)
target_link_libraries(nrhi.samples.command_list
    PUBLIC 
        nrhi
)
target_include_directories(nrhi.samples.command_list
    PUBLIC 
        nrhi
        ${CMAKE_CURRENT_SOURCE_DIR}/source
)



# nrhi.samples.command_list target dependencies
NCPP_ApplyDependencies(
    TARGET nrhi.samples.command_list
    DEPENDENT_TARGETS
        nrhi
)