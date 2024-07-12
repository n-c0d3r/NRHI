
message(STATUS "<NRHI::Drivers::DirectX12> Start implementing command list type")

include(NRHI/Utilities/EnumHelper)



NCPP_SetGlobal(NRHI_COMMAND_LIST_TYPE_ENUM_HPP_FILE_PATH "${NRHI_GENERATED_FILES_DIR}/command_list_type.hpp")
NCPP_SetGlobal(NRHI_COMMAND_LIST_TYPE_ENUM_CPP_FILE_PATH "${NRHI_GENERATED_FILES_DIR}/command_list_type.cpp")



NRHI_EnumHelper_CreateEnum(
    NAMESPACE "nrhi"
    NAME "ED_command_list_type"
    DRIVER_UPPER_CASE_NAME "DIRECTX_12"
    TARGET_HPP_FILE_PATH "${NRHI_COMMAND_LIST_TYPE_ENUM_HPP_FILE_PATH}"
    TARGET_CPP_FILE_PATH "${NRHI_COMMAND_LIST_TYPE_ENUM_CPP_FILE_PATH}"
    VALUES
        DEFAULT D3D12_COMMAND_LIST_TYPE_DIRECT
        DIRECT D3D12_COMMAND_LIST_TYPE_DIRECT
        COMPUTE D3D12_COMMAND_LIST_TYPE_COMPUTE
        BLIT D3D12_COMMAND_LIST_TYPE_COPY
)

message(STATUS "<NRHI::Drivers::DirectX12> Implement command list type done")