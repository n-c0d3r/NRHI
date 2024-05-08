
# nrhi.samples.hello_nrhi target source files
file(GLOB source_files
    "source/*.cpp"
    "source/*.hpp"
)
source_group("source" FILES ${source_files})



# Create nrhi.samples.hello_nrhi target
add_executable(nrhi.samples.hello_nrhi
    ${source_files}
)



# nrhi.samples.hello_nrhi target settings
set_target_properties(nrhi.samples.hello_nrhi
    PROPERTIES 
        FOLDER "NCoder/nrhi/samples"
        PROJECT_LABEL "hello_nrhi"
)
target_link_libraries(nrhi.samples.hello_nrhi
    PUBLIC 
        nrhi
)
target_include_directories(nrhi.samples.hello_nrhi
    PUBLIC 
        nrhi
        ${CMAKE_CURRENT_SOURCE_DIR}/source
)



# nrhi.samples.hello_nrhi target dependencies
add_dependencies(nrhi.samples.hello_nrhi 
    nrhi
)