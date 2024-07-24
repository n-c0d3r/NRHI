
# nrhi.samples.adapter target source files
file(GLOB source_files
    "source/*.cpp"
    "source/*.hpp"
)
source_group("source" FILES ${source_files})



# Create nrhi.samples.adapter target
add_executable(nrhi.samples.adapter
    ${source_files}
)



# nrhi.samples.adapter target settings
set_target_properties(nrhi.samples.adapter
    PROPERTIES 
        FOLDER "NCoder/nrhi/samples"
        PROJECT_LABEL "adapter"
)
target_link_libraries(nrhi.samples.adapter
    PUBLIC 
        nrhi
)
target_include_directories(nrhi.samples.adapter
    PUBLIC 
        nrhi
        ${CMAKE_CURRENT_SOURCE_DIR}/source
)



# nrhi.samples.adapter target dependencies
add_dependencies(nrhi.samples.adapter 
    nrhi
)