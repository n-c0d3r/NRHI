
message(STATUS "<NRHI::ConfigureDependencies> Start configuring dependencies")



#####################################################################################
#   Doxygen
#####################################################################################
find_package(Doxygen)



#####################################################################################
#   Add NMath subdirectory
#####################################################################################
if(NOT TARGET nmath)
    add_subdirectory("${NRHI_SUBMODULES_DIR}/NMath" "${NRHI_BINARY_DIR}/submodules/NMath")
endif()



#####################################################################################
#   Add NSurface subdirectory
#####################################################################################
if(NOT TARGET nsurface)
    add_subdirectory("${NRHI_SUBMODULES_DIR}/NSurface" "${NRHI_BINARY_DIR}/submodules/NSurface")
endif()



message(STATUS "<NRHI::ConfigureDependencies> Configure dependencies done")