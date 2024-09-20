
include(NCPP/Utilities/ApplyDependencies)



# nrhi.samples.root_signature target source files
file(GLOB source_files
    "source/*.cpp"
    "source/*.hpp"
)
source_group("source" FILES ${source_files})



# Create nrhi.samples.root_signature target
add_executable(nrhi.samples.root_signature
    ${source_files}
)



# nrhi.samples.root_signature target settings
set_target_properties(nrhi.samples.root_signature
    PROPERTIES 
        FOLDER "NCoder/nrhi/samples"
        PROJECT_LABEL "root_signature"
)
target_link_libraries(nrhi.samples.root_signature
    PUBLIC 
        nrhi
)
target_include_directories(nrhi.samples.root_signature
    PUBLIC 
        nrhi
        ${CMAKE_CURRENT_SOURCE_DIR}/source
)



# nrhi.samples.root_signature target dependencies
NCPP_ApplyDependencies(
    TARGET nrhi.samples.root_signature
    DEPENDENT_TARGETS
        nrhi
)