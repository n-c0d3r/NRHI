
# nrhi.samples.shader target source files
file(GLOB source_files
    "source/*.cpp"
    "source/*.hpp"
)
source_group("source" FILES ${source_files})



# Create nrhi.samples.shader target
add_executable(nrhi.samples.shader
    ${source_files}
)



# nrhi.samples.shader target settings
set_target_properties(nrhi.samples.shader
    PROPERTIES 
        FOLDER "NCoder/nrhi/samples"
        PROJECT_LABEL "shader"
)
target_link_libraries(nrhi.samples.shader
    PUBLIC 
        nrhi
)
target_include_directories(nrhi.samples.shader
    PUBLIC 
        nrhi
        ${CMAKE_CURRENT_SOURCE_DIR}/source
)



# nrhi.samples.shader target dependencies
add_dependencies(nrhi.samples.shader 
    nrhi
)