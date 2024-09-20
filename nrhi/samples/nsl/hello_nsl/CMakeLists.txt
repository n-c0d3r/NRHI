
include(NCPP/Utilities/ApplyDependencies)



# nrhi.samples.nsl.hello_nsl target source files
file(GLOB source_files
    "source/*.cpp"
    "source/*.hpp"
)
source_group("source" FILES ${source_files})



# Create nrhi.samples.nsl.hello_nsl target
add_executable(nrhi.samples.nsl.hello_nsl
    ${source_files}
)



# nrhi.samples.nsl.hello_nsl target settings
set_target_properties(nrhi.samples.nsl.hello_nsl
    PROPERTIES 
        FOLDER "NCoder/nrhi/samples"
        PROJECT_LABEL "nsl.hello_nsl"
)
target_link_libraries(nrhi.samples.nsl.hello_nsl
    PUBLIC 
        nrhi
)
target_include_directories(nrhi.samples.nsl.hello_nsl
    PUBLIC 
        nrhi
        ${CMAKE_CURRENT_SOURCE_DIR}/source
)



# nrhi.samples.nsl.hello_nsl target dependencies
NCPP_ApplyDependencies(
    TARGET nrhi.samples.nsl.hello_nsl
    DEPENDENT_TARGETS
        nrhi
)