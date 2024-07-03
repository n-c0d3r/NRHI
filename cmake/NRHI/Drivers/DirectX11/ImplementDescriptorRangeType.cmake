
message(STATUS "<NRHI::Drivers::DirectX11> Start implementing descriptor range type")

include(NRHI/Utilities/EnumHelper)



NCPP_SetGlobal(NRHI_DESCRIPTOR_RANGE_TYPE_ENUM_HPP_FILE_PATH "${NRHI_GENERATED_FILES_DIR}/descriptor_range_type.hpp")
NCPP_SetGlobal(NRHI_DESCRIPTOR_RANGE_TYPE_ENUM_CPP_FILE_PATH "${NRHI_GENERATED_FILES_DIR}/descriptor_range_type.cpp")



NRHI_EnumHelper_CreateEnum(
    NAMESPACE "nrhi"
    NAME "ED_descriptor_range_type"
    DRIVER_UPPER_CASE_NAME "DIRECTX_11"
    TARGET_HPP_FILE_PATH "${NRHI_DESCRIPTOR_RANGE_TYPE_ENUM_HPP_FILE_PATH}"
    TARGET_CPP_FILE_PATH "${NRHI_DESCRIPTOR_RANGE_TYPE_ENUM_CPP_FILE_PATH}"
    VALUES
        CBV 0
        SRV 1
        UAV 2
        SAMPLER 3
)

message(STATUS "<NRHI::Drivers::DirectX11> Implement descriptor range type done")