
message(STATUS "<NRHI::CheckNCPP> Start checking NCPP")



#####################################################################################
#   Main checkings
#####################################################################################
# check for ncpp
if(NOT TARGET ncpp)
    if(NOT EXISTS "${NRHI_SUBMODULES_DIR}/NCPP")
        if(WIN32)
            execute_process(COMMAND "${NRHI_SCRIPTS_DIR}/check_submodules.bat")
        elseif(UNIX)
            execute_process(COMMAND "${NRHI_SCRIPTS_DIR}/check_submodules.sh")
        endif()
    endif()    
endif()



message(STATUS "<NRHI::CheckNCPP> Check NCPP done")