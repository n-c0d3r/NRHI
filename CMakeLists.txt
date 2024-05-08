cmake_minimum_required(VERSION 3.12)



#####################################################################################
#   Project NRHI
#####################################################################################
project(nrhi LANGUAGES C CXX OBJC OBJCXX)



#####################################################################################
#   CMake module
#####################################################################################
set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH};${CMAKE_CURRENT_SOURCE_DIR}/cmake")



#####################################################################################
#   Local variables
#####################################################################################
set(NRHI_ROOT_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
set(NRHI_BINARY_DIR "${CMAKE_CURRENT_BINARY_DIR}")
set(NRHI_SCRIPTS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/scripts")
set(NRHI_SUBMODULES_DIR "${CMAKE_CURRENT_SOURCE_DIR}/submodules")
set(NRHI_CMAKE_MODULE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
set(NRHI_DEPENDENCIES_DIR "${CMAKE_CURRENT_BINARY_DIR}/dependencies")
set(NRHI_GENERATED_FILES_DIR "${CMAKE_CURRENT_BINARY_DIR}/nrhi/generated_files/nrhi")
set(NRHI_THIRD_PARTY_DIR "${CMAKE_CURRENT_SOURCE_DIR}/third_party")



#####################################################################################
#   Setup NCPP
#####################################################################################
include(NRHI/CheckNCPP)
include(NRHI/ConfigureNCPP)



#####################################################################################
#   Dependencies
#####################################################################################
include(NRHI/CheckDependencies)
include(NRHI/ConfigureDependencies)



#####################################################################################
#   Version
#####################################################################################
include(NCPP/Utilities/VersionHelper)
NCPP_VersionHelper_CreateVersionMacros(
    PROJECT_NAME NRHI
    OUTPUT_FILE "${NRHI_GENERATED_FILES_DIR}/version.hpp"
    MAJOR 0
    MINOR 0
    PATCH 0
)



#####################################################################################
#   Global variables
#####################################################################################
include(NCPP/Utilities/ApplyGlobal)
NCPP_ApplyGlobal(
    NRHI_ROOT_DIR
    NRHI_BINARY_DIR
    NRHI_SCRIPTS_DIR
    NRHI_SUBMODULES_DIR
    NRHI_CMAKE_MODULE_DIR 
    NRHI_DEPENDENCIES_DIR 
    NRHI_GENERATED_FILES_DIR
)



#####################################################################################
#   Documentations
#####################################################################################
option(NRHI_ENABLE_DOCUMENTATIONS "If enabled, creating a target being capable of generating documentations" OFF)
if(NRHI_ENABLE_DOCUMENTATIONS)
    option(NRHI_INTERNAL_DOCS "Generate docs for anything" OFF)

    include(NCPP/Utilities/DocsHelper)

    NCPP_DocsHelper_CreateTarget(
        NAME NRHI
        BRIEF "Ncoder's Learning Aimed C++ Render Hardware Interface Library."

        TARGET_NAME nrhi.documentations
        TARGET_FOLDER NCoder/nrhi
        TARGET_LABEL documentations

        INPUT_DIRS 
            "${CMAKE_CURRENT_SOURCE_DIR}/nrhi/source"
            "${CMAKE_CURRENT_SOURCE_DIR}/documentations"
            "${CMAKE_CURRENT_SOURCE_DIR}/README.md"
        STYLE_DIRS
            "${CMAKE_CURRENT_SOURCE_DIR}/documentations/styles"
        OUTPUT_DIR "${CMAKE_CURRENT_BINARY_DIR}/documentations"
        CWD "${CMAKE_CURRENT_SOURCE_DIR}"
        INTERNAL_DOCS ${NRHI_INTERNAL_DOCS}

        PREDEFINED 
            ${NCPP_DOCUMENTATIONS_PREDEFINED}

        MAIN_PAGE "${CMAKE_CURRENT_SOURCE_DIR}/README.md"
    )
endif()



#####################################################################################
#   Add main directory
#####################################################################################
add_subdirectory(nrhi)



include(CPack)