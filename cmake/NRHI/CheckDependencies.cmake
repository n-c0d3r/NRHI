
message(STATUS "<NRHI::CheckDependencies> Start checking dependencies")



#####################################################################################
#   Includes
#####################################################################################
include(NCPP/Utilities/GitHelper)



#####################################################################################
#   Setup directory for dependencies to be downloaded into
#####################################################################################
if(NOT EXISTS ${NRHI_DEPENDENCIES_DIR})
    file(MAKE_DIRECTORY ${NRHI_DEPENDENCIES_DIR})
endif()



#####################################################################################
#   NMath checking
#####################################################################################
if(NOT TARGET nmath)
    if(NOT EXISTS "${NRHI_SUBMODULES_DIR}/NMath")
        if(WIN32)
            execute_process(COMMAND "${NRHI_SCRIPTS_DIR}/check_submodules.bat")
        elseif(UNIX)
            execute_process(COMMAND "${NRHI_SCRIPTS_DIR}/check_submodules.sh")
        endif()
    endif()    
endif()

#####################################################################################
#   NSurface checking
#####################################################################################
if(NOT TARGET nsurface)
    if(NOT EXISTS "${NRHI_SUBMODULES_DIR}/NSurface")
        if(WIN32)
            execute_process(COMMAND "${NRHI_SCRIPTS_DIR}/check_submodules.bat")
        elseif(UNIX)
            execute_process(COMMAND "${NRHI_SCRIPTS_DIR}/check_submodules.sh")
        endif()
    endif()    
endif()

#####################################################################################
#   SPIRV-Cross checking
#####################################################################################
#if(NOT TARGET spirv-cross)
#    if(NOT EXISTS "${NRHI_DEPENDENCIES_DIR}/SPIRV-Cross")
#        file(MAKE_DIRECTORY "${NRHI_DEPENDENCIES_DIR}/SPIRV-Cross")
#        NCPP_GitHelper_Clone(
#            PROJECT_NAME "SPIRV-Cross"
#            GIT_URL "https://github.com/KhronosGroup/SPIRV-Cross"
#            GIT_COMMIT "1ddd8b629c4a18994056a0df9095ccb108e84c9e"
#            GIT_BRANCH "main"
#            DIRECTORY "${NRHI_DEPENDENCIES_DIR}"
#        )
#    endif()
#endif()

#####################################################################################
#   DirectXShaderCompiler checking
#####################################################################################
if(NOT TARGET dxc)
    if(NOT EXISTS "${NRHI_DEPENDENCIES_DIR}/DirectXShaderCompiler")
        file(MAKE_DIRECTORY "${NRHI_DEPENDENCIES_DIR}/DirectXShaderCompiler")
        NCPP_GitHelper_Clone(
            PROJECT_NAME "DirectXShaderCompiler"
            GIT_URL "https://github.com/microsoft/DirectXShaderCompiler"
            GIT_COMMIT "9bfbee6c44deecf28111879f8d21fa15773181ef"
            GIT_BRANCH "main"
            DIRECTORY "${NRHI_DEPENDENCIES_DIR}"
        )
    endif()
endif()



message(STATUS "<NRHI::CheckDependencies> Check dependencies done")