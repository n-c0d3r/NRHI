
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



#####################################################################################
#   Add SPIRV-Cross subdirectory
#####################################################################################
if(NOT TARGET spirv-cross)
    add_subdirectory("${NRHI_SUBMODULES_DIR}/SPIRV-Cross" "${NRHI_BINARY_DIR}/submodules/SPIRV-Cross")
endif()



#####################################################################################
#   Add DirectXShaderCompiler subdirectory
#####################################################################################
#if(NOT TARGET spirv-cross)
    if (DXC_COVERAGE)
        set(LLVM_BUILD_INSTRUMENTED_COVERAGE ON)
        set(LLVM_PROFILE_DATA_DIR "${CMAKE_BINARY_DIR}/profile")
        set(LLVM_CODE_COVERAGE_TARGETS "dxc;dxcompiler")
        set(LLVM_CODE_COVERAGE_TEST_TARGETS "check-all")
    endif()

    set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
    set(LLVM_APPEND_VC_REV ON)
    set(LLVM_DEFAULT_TARGET_TRIPLE "dxil-ms-dx")
    set(LLVM_ENABLE_EH ON)
    set(LLVM_ENABLE_RTTI ON)
    set(LLVM_INCLUDE_DOCS OFF)
    set(LLVM_INCLUDE_EXAMPLES OFF)
    set(LLVM_OPTIMIZED_TABLEGEN OFF)
    set(LLVM_TARGETS_TO_BUILD "None")
    set(LIBCLANG_BUILD_STATIC ON)
    set(CLANG_BUILD_EXAMPLES OFF)
    set(CLANG_CL OFF)
    set(CLANG_ENABLE_ARCMT OFF)
    set(CLANG_ENABLE_STATIC_ANALYZER OFF)
    set(HLSL_INCLUDE_TESTS ON)
    set(ENABLE_SPIRV_CODEGEN ON)
    set(SPIRV_BUILD_TESTS ON)
    set(LLVM_ENABLE_TERMINFO OFF)

    add_subdirectory("${NRHI_SUBMODULES_DIR}/DirectXShaderCompiler" "${NRHI_BINARY_DIR}/submodules/DirectXShaderCompiler")
#endif()



message(STATUS "<NRHI::ConfigureDependencies> Configure dependencies done")