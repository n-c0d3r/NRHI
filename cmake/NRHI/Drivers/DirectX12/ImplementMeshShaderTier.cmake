
message(STATUS "<NRHI::Drivers::DirectX12> Start implementing mesh shader tier")

include(NRHI/Utilities/EnumHelper)



NCPP_SetGlobal(NRHI_MESH_SHADER_TIER_ENUM_HPP_FILE_PATH "${NRHI_GENERATED_FILES_DIR}/mesh_shader_tier.hpp")
NCPP_SetGlobal(NRHI_MESH_SHADER_TIER_ENUM_CPP_FILE_PATH "${NRHI_GENERATED_FILES_DIR}/mesh_shader_tier.cpp")



NRHI_EnumHelper_CreateEnum(
    NAMESPACE "nrhi"
    NAME "ED_mesh_shader_tier"
    DRIVER_UPPER_CASE_NAME "DIRECTX_12"
    TARGET_HPP_FILE_PATH "${NRHI_MESH_SHADER_TIER_ENUM_HPP_FILE_PATH}"
    TARGET_CPP_FILE_PATH "${NRHI_MESH_SHADER_TIER_ENUM_CPP_FILE_PATH}"
    VALUES
        NOT_SUPPORTED D3D12_MESH_SHADER_TIER_NOT_SUPPORTED
        A D3D12_MESH_SHADER_TIER_1
)

message(STATUS "<NRHI::Drivers::DirectX12> Implement mesh shader tier done")