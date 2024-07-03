
message(STATUS "<NRHI::Drivers::DirectX11> Start implementing root param type")

include(NRHI/Utilities/EnumHelper)



NCPP_SetGlobal(NRHI_ROOT_PARAM_TYPE_ENUM_HPP_FILE_PATH "${NRHI_GENERATED_FILES_DIR}/root_param_type.hpp")
NCPP_SetGlobal(NRHI_ROOT_PARAM_TYPE_ENUM_CPP_FILE_PATH "${NRHI_GENERATED_FILES_DIR}/root_param_type.cpp")



NRHI_EnumHelper_CreateEnum(
    NAMESPACE "nrhi"
    NAME "ED_root_param_type"
    DRIVER_UPPER_CASE_NAME "DIRECTX_11"
    TARGET_HPP_FILE_PATH "${NRHI_ROOT_PARAM_TYPE_ENUM_HPP_FILE_PATH}"
    TARGET_CPP_FILE_PATH "${NRHI_ROOT_PARAM_TYPE_ENUM_CPP_FILE_PATH}"
    VALUES
        DESCRIPTOR_TABLE 0
        CBV 1
        SRV 2
        UAV 3
)

message(STATUS "<NRHI::Drivers::DirectX11> Implement root param type done")