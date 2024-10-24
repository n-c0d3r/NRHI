
message(STATUS "<NRHI::Drivers::DirectX12> Start implementing resource heap tier")

include(NRHI/Utilities/EnumHelper)



NCPP_SetGlobal(NRHI_RESOURCE_HEAP_TIER_ENUM_HPP_FILE_PATH "${NRHI_GENERATED_FILES_DIR}/resource_heap_tier.hpp")
NCPP_SetGlobal(NRHI_RESOURCE_HEAP_TIER_ENUM_CPP_FILE_PATH "${NRHI_GENERATED_FILES_DIR}/resource_heap_tier.cpp")



NRHI_EnumHelper_CreateEnum(
    NAMESPACE "nrhi"
    NAME "ED_resource_heap_tier"
    DRIVER_UPPER_CASE_NAME "DIRECTX_12"
    TARGET_HPP_FILE_PATH "${NRHI_RESOURCE_HEAP_TIER_ENUM_HPP_FILE_PATH}"
    TARGET_CPP_FILE_PATH "${NRHI_RESOURCE_HEAP_TIER_ENUM_CPP_FILE_PATH}"
    VALUES
        A D3D12_RESOURCE_HEAP_TIER_1
        B D3D12_RESOURCE_HEAP_TIER_2
        DEFAULT D3D12_RESOURCE_HEAP_TIER_1
)

message(STATUS "<NRHI::Drivers::DirectX12> Implement resource heap tier done")