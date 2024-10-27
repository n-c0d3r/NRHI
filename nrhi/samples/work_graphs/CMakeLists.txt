
include(NCPP/Utilities/ApplyDependencies)



# nrhi.samples.work_graphs target source files
file(GLOB source_files
    "source/*.cpp"
    "source/*.hpp"
)
source_group("source" FILES ${source_files})



# Create nrhi.samples.work_graphs target
add_executable(nrhi.samples.work_graphs
    ${source_files}
)



# nrhi.samples.work_graphs target settings
set_target_properties(nrhi.samples.work_graphs
    PROPERTIES 
        FOLDER "NCoder/nrhi/samples"
        PROJECT_LABEL "work_graphs"
)
target_link_libraries(nrhi.samples.work_graphs
    PUBLIC 
        nrhi
)
target_include_directories(nrhi.samples.work_graphs
    PUBLIC 
        nrhi
        ${CMAKE_CURRENT_SOURCE_DIR}/source
)



# nrhi.samples.work_graphs target dependencies
NCPP_ApplyDependencies(
    TARGET nrhi.samples.work_graphs
    DEPENDENT_TARGETS
        nrhi
)