
# nrhi.samples.hello_nsl target source files
file(GLOB source_files
    "source/*.cpp"
    "source/*.hpp"
)
source_group("source" FILES ${source_files})



# Create nrhi.samples.hello_nsl target
add_executable(nrhi.samples.hello_nsl
    ${source_files}
)



# nrhi.samples.hello_nsl target settings
set_target_properties(nrhi.samples.hello_nsl
    PROPERTIES 
        FOLDER "NCoder/nrhi/samples"
        PROJECT_LABEL "hello_nsl"
)
target_link_libraries(nrhi.samples.hello_nsl
    PUBLIC 
        nrhi
)
target_include_directories(nrhi.samples.hello_nsl
    PUBLIC 
        nrhi
        ${CMAKE_CURRENT_SOURCE_DIR}/source
)



# nrhi.samples.hello_nsl target dependencies
add_dependencies(nrhi.samples.hello_nsl 
    nrhi
)