
include(NCPP/Utilities/ApplyDependencies)



# nrhi.samples.device target source files
file(GLOB source_files
    "source/*.cpp"
    "source/*.hpp"
)
source_group("source" FILES ${source_files})



# Create nrhi.samples.device target
add_executable(nrhi.samples.device
    ${source_files}
)



# nrhi.samples.device target settings
set_target_properties(nrhi.samples.device
    PROPERTIES 
        FOLDER "NCoder/nrhi/samples"
        PROJECT_LABEL "device"
)
target_link_libraries(nrhi.samples.device
    PUBLIC 
        nrhi
)
target_include_directories(nrhi.samples.device
    PUBLIC 
        nrhi
        ${CMAKE_CURRENT_SOURCE_DIR}/source
)



# nrhi.samples.device target dependencies
NCPP_ApplyDependencies(
    TARGET nrhi.samples.device
    DEPENDENT_TARGETS
        nrhi
)