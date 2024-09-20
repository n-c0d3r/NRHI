
message(STATUS "<NRHI::ConfigureDependencies> Start configuring dependencies")



include(NCPP/Utilities/SetGlobal)



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

#####################################################################################
#   Add SPIRV-Cross subdirectory
#####################################################################################
#if(NOT TARGET spirv-cross)
#    add_subdirectory("${NRHI_DEPENDENCIES_DIR}/SPIRV-Cross" "${NRHI_DEPENDENCIES_DIR}/SPIRV-Cross/build")
#endif()

#####################################################################################
#   Add DirectXShaderCompiler subdirectory
#####################################################################################
if(NOT TARGET dxc)
    NCPP_SetGlobal(HLSL_DISABLE_SOURCE_GENERATION OFF)
    NCPP_SetGlobal(CLANG_INCLUDE_TESTS OFF)
    NCPP_SetGlobal(LLVM_BUILD_TESTS OFF)

    if (DXC_COVERAGE)
        NCPP_SetGlobal(LLVM_BUILD_INSTRUMENTED_COVERAGE ON)
        NCPP_SetGlobal(LLVM_PROFILE_DATA_DIR "${CMAKE_BINARY_DIR}/profile")
        NCPP_SetGlobal(LLVM_CODE_COVERAGE_TARGETS "dxc;dxcompiler")
        NCPP_SetGlobal(LLVM_CODE_COVERAGE_TEST_TARGETS "")
    endif()

    NCPP_SetGlobal(CMAKE_EXPORT_COMPILE_COMMANDS ON)
    NCPP_SetGlobal(LLVM_APPEND_VC_REV ON)
    NCPP_SetGlobal(LLVM_DEFAULT_TARGET_TRIPLE "dxil-ms-dx")
    NCPP_SetGlobal(LLVM_ENABLE_EH ON)
    NCPP_SetGlobal(LLVM_ENABLE_RTTI ON)
    NCPP_SetGlobal(LLVM_INCLUDE_DOCS OFF)
    NCPP_SetGlobal(LLVM_INCLUDE_EXAMPLES OFF)
    NCPP_SetGlobal(LLVM_OPTIMIZED_TABLEGEN OFF)
    NCPP_SetGlobal(LLVM_TARGETS_TO_BUILD "None")
    NCPP_SetGlobal(LIBCLANG_BUILD_STATIC ON)
    NCPP_SetGlobal(CLANG_BUILD_EXAMPLES OFF)
    NCPP_SetGlobal(CLANG_CL OFF)
    NCPP_SetGlobal(CLANG_ENABLE_ARCMT OFF)
    NCPP_SetGlobal(CLANG_ENABLE_STATIC_ANALYZER OFF)
    NCPP_SetGlobal(HLSL_INCLUDE_TESTS OFF)
    NCPP_SetGlobal(ENABLE_SPIRV_CODEGEN OFF)
    NCPP_SetGlobal(SPIRV_BUILD_TESTS OFF)
    NCPP_SetGlobal(LLVM_ENABLE_TERMINFO OFF)

    add_subdirectory("${NRHI_DEPENDENCIES_DIR}/DirectXShaderCompiler" "${NRHI_DEPENDENCIES_DIR}/DirectXShaderCompiler/build")
endif()



message(STATUS "<NRHI::ConfigureDependencies> Configure dependencies done")