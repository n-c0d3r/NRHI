
message(STATUS "<NRHI::Drivers::DirectX12> Start implementing work graphs tier")

include(NRHI/Utilities/EnumHelper)



NCPP_SetGlobal(NRHI_WORK_GRAPHS_TIER_ENUM_HPP_FILE_PATH "${NRHI_GENERATED_FILES_DIR}/work_graphs_tier.hpp")
NCPP_SetGlobal(NRHI_WORK_GRAPHS_TIER_ENUM_CPP_FILE_PATH "${NRHI_GENERATED_FILES_DIR}/work_graphs_tier.cpp")



NRHI_EnumHelper_CreateEnum(
    NAMESPACE "nrhi"
    NAME "ED_work_graphs_tier"
    DRIVER_UPPER_CASE_NAME "DIRECTX_12"
    TARGET_HPP_FILE_PATH "${NRHI_WORK_GRAPHS_TIER_ENUM_HPP_FILE_PATH}"
    TARGET_CPP_FILE_PATH "${NRHI_WORK_GRAPHS_TIER_ENUM_CPP_FILE_PATH}"
    VALUES
        NOT_SUPPORTED D3D12_WORK_GRAPHS_TIER_NOT_SUPPORTED
        A D3D12_WORK_GRAPHS_TIER_1_0
)

message(STATUS "<NRHI::Drivers::DirectX12> Implement work graphs tier done")