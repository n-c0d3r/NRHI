
message(STATUS "<NRHI::Drivers::DirectX11> Start implementing command list type")

include(NRHI/Utilities/EnumHelper)



NCPP_SetGlobal(NRHI_COMMAND_LIST_TYPE_ENUM_HPP_FILE_PATH "${NRHI_GENERATED_FILES_DIR}/command_list_type.hpp")
NCPP_SetGlobal(NRHI_COMMAND_LIST_TYPE_ENUM_CPP_FILE_PATH "${NRHI_GENERATED_FILES_DIR}/command_list_type.cpp")



NRHI_EnumHelper_CreateEnum(
    NAMESPACE "nrhi"
    NAME "ED_command_list_type"
    DRIVER_UPPER_CASE_NAME "DIRECTX_11"
    TARGET_HPP_FILE_PATH "${NRHI_COMMAND_LIST_TYPE_ENUM_HPP_FILE_PATH}"
    TARGET_CPP_FILE_PATH "${NRHI_COMMAND_LIST_TYPE_ENUM_CPP_FILE_PATH}"
    VALUES
        DEFAULT 0
        DIRECT 0
        COMPUTE 1
        BLIT 2
)

message(STATUS "<NRHI::Drivers::DirectX11> Implement command list type done")