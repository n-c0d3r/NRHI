
message(STATUS "<NRHI::Drivers::DirectX12> Start implementing blend operation")

include(NRHI/Utilities/EnumHelper)



NCPP_SetGlobal(NRHI_BLEND_OPERATION_ENUM_HPP_FILE_PATH "${NRHI_GENERATED_FILES_DIR}/blend_operation.hpp")
NCPP_SetGlobal(NRHI_BLEND_OPERATION_ENUM_CPP_FILE_PATH "${NRHI_GENERATED_FILES_DIR}/blend_operation.cpp")



NRHI_EnumHelper_CreateEnum(
    NAMESPACE "nrhi"
    NAME "ED_blend_operation"
    DRIVER_UPPER_CASE_NAME "DIRECTX_12"
    TARGET_HPP_FILE_PATH "${NRHI_BLEND_OPERATION_ENUM_HPP_FILE_PATH}"
    TARGET_CPP_FILE_PATH "${NRHI_BLEND_OPERATION_ENUM_CPP_FILE_PATH}"
    VALUES
        ADD D3D12_BLEND_OP_ADD
        SUBTRACT D3D12_BLEND_OP_SUBTRACT
        REV_SUBTRACT D3D12_BLEND_OP_REV_SUBTRACT
        MIN D3D12_BLEND_OP_MIN
        MAX D3D12_BLEND_OP_MAX
)

message(STATUS "<NRHI::Drivers::DirectX12> Implement blend operation done")