
message(STATUS "<NRHI::Drivers::DirectX11> Start implementing pipeline type")

include(NRHI/Utilities/EnumHelper)



NCPP_SetGlobal(NRHI_PIPELINE_TYPE_ENUM_HPP_FILE_PATH "${NRHI_GENERATED_FILES_DIR}/pipeline_type.hpp")
NCPP_SetGlobal(NRHI_PIPELINE_TYPE_ENUM_CPP_FILE_PATH "${NRHI_GENERATED_FILES_DIR}/pipeline_type.cpp")



NRHI_EnumHelper_CreateEnum(
    NAMESPACE "nrhi"
    NAME "E_pipeline_type"
    DRIVER_UPPER_CASE_NAME "DIRECTX_11"
    TARGET_HPP_FILE_PATH "${NRHI_PIPELINE_TYPE_ENUM_HPP_FILE_PATH}"
    TARGET_CPP_FILE_PATH "${NRHI_PIPELINE_TYPE_ENUM_CPP_FILE_PATH}"
    VALUES
        NONE 0
        GRAPHICS 1
        COMPUTE 2
)

message(STATUS "<NRHI::Drivers::DirectX11> Implement pipeline type done")