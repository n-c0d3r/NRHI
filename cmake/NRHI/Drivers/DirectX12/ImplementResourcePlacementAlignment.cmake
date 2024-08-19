
message(STATUS "<NRHI::Drivers::DirectX12> Start implementing resource placement alignment")

include(NRHI/Utilities/EnumHelper)



NCPP_SetGlobal(NRHI_RESOURCE_PLACEMENT_ALIGNMENT_ENUM_HPP_FILE_PATH "${NRHI_GENERATED_FILES_DIR}/resource_placement_alignment.hpp")
NCPP_SetGlobal(NRHI_RESOURCE_PLACEMENT_ALIGNMENT_ENUM_CPP_FILE_PATH "${NRHI_GENERATED_FILES_DIR}/resource_placement_alignment.cpp")



NRHI_EnumHelper_CreateEnum(
    NAMESPACE "nrhi"
    NAME "ED_resource_placement_alignment"
    DRIVER_UPPER_CASE_NAME "DIRECTX_12"
    TYPE "u64"
    TARGET_HPP_FILE_PATH "${NRHI_RESOURCE_PLACEMENT_ALIGNMENT_ENUM_HPP_FILE_PATH}"
    TARGET_CPP_FILE_PATH "${NRHI_RESOURCE_PLACEMENT_ALIGNMENT_ENUM_CPP_FILE_PATH}"
    VALUES
        DEFAULT 65536
        MSAA D3D12_DEFAULT_MSAA_RESOURCE_PLACEMENT_ALIGNMENT
)

message(STATUS "<NRHI::Drivers::DirectX12> Implement resource placement alignment done")