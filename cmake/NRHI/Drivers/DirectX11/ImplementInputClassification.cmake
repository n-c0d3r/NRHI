
message(STATUS "<NRHI::Drivers::DirectX11> Start implementing input classification")

include(NRHI/Utilities/EnumHelper)



NCPP_SetGlobal(NRHI_INPUT_CLASSIFICATION_ENUM_HPP_FILE_PATH "${NRHI_GENERATED_FILES_DIR}/input_classification.hpp")
NCPP_SetGlobal(NRHI_INPUT_CLASSIFICATION_ENUM_CPP_FILE_PATH "${NRHI_GENERATED_FILES_DIR}/input_classification.cpp")



NRHI_EnumHelper_CreateEnum(
    NAMESPACE "nrhi"
    NAME "ED_input_classification"
    DRIVER_UPPER_CASE_NAME "DIRECTX_11"
    TARGET_HPP_FILE_PATH "${NRHI_INPUT_CLASSIFICATION_ENUM_HPP_FILE_PATH}"
    TARGET_CPP_FILE_PATH "${NRHI_INPUT_CLASSIFICATION_ENUM_CPP_FILE_PATH}"
    VALUES
        PER_VERTEX_DATA D3D11_INPUT_PER_VERTEX_DATA
        PER_INSTANCE_DATA D3D11_INPUT_PER_INSTANCE_DATA
)

message(STATUS "<NRHI::Drivers::DirectX11> Implement input classification done")