
message(STATUS "<NRHI::Drivers::DirectX11> Start implementing resource flag")

include(NRHI/Utilities/EnumHelper)



NCPP_SetGlobal(NRHI_RESOURCE_FLAG_ENUM_HPP_FILE_PATH "${NRHI_GENERATED_FILES_DIR}/resource_flag.hpp")
NCPP_SetGlobal(NRHI_RESOURCE_FLAG_ENUM_CPP_FILE_PATH "${NRHI_GENERATED_FILES_DIR}/resource_flag.cpp")



NRHI_EnumHelper_CreateEnum(
    NAMESPACE "nrhi"
    NAME "E_resource_flag"
    TYPE "ncpp::i64"
    DRIVER_UPPER_CASE_NAME "DIRECTX_11"
    DRIVER_LOWER_CASE_NAME "directx_11"
    TARGET_HPP_FILE_PATH "${NRHI_RESOURCE_FLAG_ENUM_HPP_FILE_PATH}"
    TARGET_CPP_FILE_PATH "${NRHI_RESOURCE_FLAG_ENUM_CPP_FILE_PATH}"
    VALUES
        DEFAULT 0
        ALLOW_RENDER_TARGET 0x1
        ALLOW_DEPTH_STENCIL 0x2
        ALLOW_UNORDERED_ACCESS 0x4
        DENY_SHADER_RESOURCE 0x8
)

message(STATUS "<NRHI::Drivers::DirectX11> Implement resource flag done")