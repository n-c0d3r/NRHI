
message(STATUS "<NRHI::Drivers::DirectX12> Start implementing state object")

include(NRHI/Utilities/FunctionHelper)



NCPP_SetGlobal(NRHI_state_object_HPP_FILE_PATH "${NRHI_GENERATED_FILES_DIR}/state_object.hpp")
NCPP_SetGlobal(NRHI_state_object_CPP_FILE_PATH "${NRHI_GENERATED_FILES_DIR}/state_object.cpp")



NRHI_FunctionHelper_CreateFunctionClass(
    NAMESPACE "nrhi"
    NAME "H_state_object"
    DRIVER_SPECIFIC_NAME "HD_directx12_state_object"
    DRIVER_UPPER_CASE_NAME "DIRECTX_12"
    TARGET_HPP_FILE_PATH "${NRHI_state_object_HPP_FILE_PATH}"
    TARGET_CPP_FILE_PATH "${NRHI_state_object_CPP_FILE_PATH}"
    INCLUDES
        "<nrhi/directx12/state_object.hpp>"
    ADDITIONAL_CODE
        "namespace nrhi {
        }"
    VALUES
)

message(STATUS "<NRHI::Drivers::DirectX12> Implement state object done")