
message(STATUS "<NRHI::Drivers::DirectX12> Start implementing cull mode")

include(NRHI/Utilities/EnumHelper)



NCPP_SetGlobal(NRHI_CULL_MODE_ENUM_HPP_FILE_PATH "${NRHI_GENERATED_FILES_DIR}/cull_mode.hpp")
NCPP_SetGlobal(NRHI_CULL_MODE_ENUM_CPP_FILE_PATH "${NRHI_GENERATED_FILES_DIR}/cull_mode.cpp")



NRHI_EnumHelper_CreateEnum(
    NAMESPACE "nrhi"
    NAME "ED_cull_mode"
    DRIVER_UPPER_CASE_NAME "DIRECTX_12"
    TARGET_HPP_FILE_PATH "${NRHI_CULL_MODE_ENUM_HPP_FILE_PATH}"
    TARGET_CPP_FILE_PATH "${NRHI_CULL_MODE_ENUM_CPP_FILE_PATH}"
    VALUES
        NONE D3D12_CULL_MODE_NONE
        FRONT D3D12_CULL_MODE_FRONT
        BACK D3D12_CULL_MODE_BACK
)

message(STATUS "<NRHI::Drivers::DirectX12> Implement cull mode done")