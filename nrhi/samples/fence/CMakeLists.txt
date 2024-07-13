
# nrhi.samples.fence target source files
file(GLOB source_files
    "source/*.cpp"
    "source/*.hpp"
)
source_group("source" FILES ${source_files})



# Create nrhi.samples.fence target
add_executable(nrhi.samples.fence
    ${source_files}
)



# nrhi.samples.fence target settings
set_target_properties(nrhi.samples.fence
    PROPERTIES 
        FOLDER "NCoder/nrhi/samples"
        PROJECT_LABEL "fence"
)
target_link_libraries(nrhi.samples.fence
    PUBLIC 
        nrhi
)
target_include_directories(nrhi.samples.fence
    PUBLIC 
        nrhi
        ${CMAKE_CURRENT_SOURCE_DIR}/source
)



# nrhi.samples.fence target dependencies
add_dependencies(nrhi.samples.fence 
    nrhi
)