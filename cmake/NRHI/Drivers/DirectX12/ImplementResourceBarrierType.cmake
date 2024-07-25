
message(STATUS "<NRHI::Drivers::DirectX12> Start implementing resource barrier type")

include(NRHI/Utilities/EnumHelper)



NCPP_SetGlobal(NRHI_RESOURCE_BARRIER_TYPE_ENUM_HPP_FILE_PATH "${NRHI_GENERATED_FILES_DIR}/resource_barrier_type.hpp")
NCPP_SetGlobal(NRHI_RESOURCE_BARRIER_TYPE_ENUM_CPP_FILE_PATH "${NRHI_GENERATED_FILES_DIR}/resource_barrier_type.cpp")



NRHI_EnumHelper_CreateEnum(
    NAMESPACE "nrhi"
    NAME "ED_resource_barrier_type"
    DRIVER_UPPER_CASE_NAME "DIRECTX_12"
    TARGET_HPP_FILE_PATH "${NRHI_RESOURCE_BARRIER_TYPE_ENUM_HPP_FILE_PATH}"
    TARGET_CPP_FILE_PATH "${NRHI_RESOURCE_BARRIER_TYPE_ENUM_CPP_FILE_PATH}"
    VALUES
        TRANSITION D3D12_RESOURCE_BARRIER_TYPE_TRANSITION
        ALIASING D3D12_RESOURCE_BARRIER_TYPE_ALIASING
        UAV D3D12_RESOURCE_BARRIER_TYPE_UAV
)

message(STATUS "<NRHI::Drivers::DirectX12> Implement resource barrier type done")