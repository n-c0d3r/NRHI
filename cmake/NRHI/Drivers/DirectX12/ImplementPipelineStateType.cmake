
message(STATUS "<NRHI::Drivers::DirectX12> Start implementing pipeline state type")

include(NRHI/Utilities/EnumHelper)



NCPP_SetGlobal(NRHI_PIPELINE_STATE_TYPE_ENUM_HPP_FILE_PATH "${NRHI_GENERATED_FILES_DIR}/pipeline_state_type.hpp")
NCPP_SetGlobal(NRHI_PIPELINE_STATE_TYPE_ENUM_CPP_FILE_PATH "${NRHI_GENERATED_FILES_DIR}/pipeline_state_type.cpp")



NRHI_EnumHelper_CreateEnum(
    NAMESPACE "nrhi"
    NAME "ED_pipeline_state_type"
    DRIVER_UPPER_CASE_NAME "DIRECTX_12"
    TARGET_HPP_FILE_PATH "${NRHI_PIPELINE_STATE_TYPE_ENUM_HPP_FILE_PATH}"
    TARGET_CPP_FILE_PATH "${NRHI_PIPELINE_STATE_TYPE_ENUM_CPP_FILE_PATH}"
    VALUES
        NONE 0
        GRAPHICS 1
        COMPUTE 2
)

message(STATUS "<NRHI::Drivers::DirectX12> Implement pipeline state type done")