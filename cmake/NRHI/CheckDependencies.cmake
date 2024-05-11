
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
if(NOT TARGET spirv-cross)
    if(NOT EXISTS "${NRHI_SUBMODULES_DIR}/SPIRV-Cross")
        if(WIN32)
            execute_process(COMMAND "${NRHI_SCRIPTS_DIR}/check_submodules.bat")
        elseif(UNIX)
            execute_process(COMMAND "${NRHI_SCRIPTS_DIR}/check_submodules.sh")
        endif()
    endif()
endif()

#####################################################################################
#   DirectXShaderCompiler checking
#####################################################################################
#if(NOT TARGET spirv-cross)
    if(NOT EXISTS "${NRHI_SUBMODULES_DIR}/DirectXShaderCompiler")
        if(WIN32)
            execute_process(COMMAND "${NRHI_SCRIPTS_DIR}/check_submodules.bat")
        elseif(UNIX)
            execute_process(COMMAND "${NRHI_SCRIPTS_DIR}/check_submodules.sh")
        endif()
        execute_process(COMMAND "${NRHI_SUBMODULES_DIR}/DirectXShaderCompiler/external")
    endif()
#endif()



message(STATUS "<NRHI::CheckDependencies> Check dependencies done")