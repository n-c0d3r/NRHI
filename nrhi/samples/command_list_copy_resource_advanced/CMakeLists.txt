
# nrhi.samples.command_list_copy_resource_advanced target source files
file(GLOB source_files
    "source/*.cpp"
    "source/*.hpp"
)
source_group("source" FILES ${source_files})



# Create nrhi.samples.command_list_copy_resource_advanced target
add_executable(nrhi.samples.command_list_copy_resource_advanced
    ${source_files}
)



# nrhi.samples.command_list_copy_resource_advanced target settings
set_target_properties(nrhi.samples.command_list_copy_resource_advanced
    PROPERTIES 
        FOLDER "NCoder/nrhi/samples"
        PROJECT_LABEL "command_list_copy_resource_advanced"
)
target_link_libraries(nrhi.samples.command_list_copy_resource_advanced
    PUBLIC 
        nrhi
)
target_include_directories(nrhi.samples.command_list_copy_resource_advanced
    PUBLIC 
        nrhi
        ${CMAKE_CURRENT_SOURCE_DIR}/source
)



# nrhi.samples.command_list_copy_resource_advanced target dependencies
add_dependencies(nrhi.samples.command_list_copy_resource_advanced 
    nrhi
)