
include(NCPP/Utilities/ApplyDependencies)



# nrhi.samples.resource.textures target source files
file(GLOB source_files
    "source/*.cpp"
    "source/*.hpp"
)
source_group("source" FILES ${source_files})



# Create nrhi.samples.resource.textures target
add_executable(nrhi.samples.resource.textures
    ${source_files}
)



# nrhi.samples.resource.textures target settings
set_target_properties(nrhi.samples.resource.textures
    PROPERTIES 
        FOLDER "NCoder/nrhi/samples"
        PROJECT_LABEL "resource.textures"
)
target_link_libraries(nrhi.samples.resource.textures
    PUBLIC 
        nrhi
)
target_include_directories(nrhi.samples.resource.textures
    PUBLIC 
        nrhi
        ${CMAKE_CURRENT_SOURCE_DIR}/source
)



# nrhi.samples.resource.textures target dependencies
NCPP_ApplyDependencies(
    TARGET nrhi.samples.resource.textures
    DEPENDENT_TARGETS
        nrhi
)