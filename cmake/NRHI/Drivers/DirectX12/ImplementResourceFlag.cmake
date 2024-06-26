
message(STATUS "<NRHI::Drivers::DirectX12> Start implementing resource flag")

include(NRHI/Utilities/EnumHelper)



NCPP_SetGlobal(NRHI_RESOURCE_FLAG_ENUM_HPP_FILE_PATH "${NRHI_GENERATED_FILES_DIR}/resource_flag.hpp")
NCPP_SetGlobal(NRHI_RESOURCE_FLAG_ENUM_CPP_FILE_PATH "${NRHI_GENERATED_FILES_DIR}/resource_flag.cpp")



NRHI_EnumHelper_CreateEnum(
    NAMESPACE "nrhi"
    NAME "E_resource_flag"
    DRIVER_UPPER_CASE_NAME "DIRECTX_12"
    TARGET_HPP_FILE_PATH "${NRHI_RESOURCE_FLAG_ENUM_HPP_FILE_PATH}"
    TARGET_CPP_FILE_PATH "${NRHI_RESOURCE_FLAG_ENUM_CPP_FILE_PATH}"
    VALUES
        DEFAULT 0
        ALLOW_RENDER_TARGET D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET
        ALLOW_DEPTH_STENCIL D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL
        ALLOW_UNORDERED_ACCESS D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS
        DENY_SHADER_RESOURCE D3D12_RESOURCE_FLAG_DENY_SHADER_RESOURCE
)

message(STATUS "<NRHI::Drivers::DirectX12> Implement resource flag done")