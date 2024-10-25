
include(NCPP/Utilities/ApplyDependencies)



# nrhi.samples.nsl.hello_nsl_6_6 target source files
file(GLOB source_files
    "source/*.cpp"
    "source/*.hpp"
)
source_group("source" FILES ${source_files})



# Create nrhi.samples.nsl.hello_nsl_6_6 target
add_executable(nrhi.samples.nsl.hello_nsl_6_6
    ${source_files}
)



# nrhi.samples.nsl.hello_nsl_6_6 target settings
set_target_properties(nrhi.samples.nsl.hello_nsl_6_6
    PROPERTIES 
        FOLDER "NCoder/nrhi/samples"
        PROJECT_LABEL "nsl.hello_nsl_6_6"
)
target_link_libraries(nrhi.samples.nsl.hello_nsl_6_6
    PUBLIC 
        nrhi
)
target_include_directories(nrhi.samples.nsl.hello_nsl_6_6
    PUBLIC 
        nrhi
        ${CMAKE_CURRENT_SOURCE_DIR}/source
)



# nrhi.samples.nsl.hello_nsl_6_6 target dependencies
NCPP_ApplyDependencies(
    TARGET nrhi.samples.nsl.hello_nsl_6_6
    DEPENDENT_TARGETS
        nrhi
)