
message(STATUS "<NRHI::Drivers::DirectX12> Start implementing resource state helper")

include(NRHI/Utilities/FunctionHelper)



NCPP_SetGlobal(NRHI_RESOURCE_STATE_HPP_FILE_PATH "${NRHI_GENERATED_FILES_DIR}/resource_state_helper.hpp")
NCPP_SetGlobal(NRHI_RESOURCE_STATE_CPP_FILE_PATH "${NRHI_GENERATED_FILES_DIR}/resource_state_helper.cpp")



NRHI_FunctionHelper_CreateFunctionClass(
    NAMESPACE "nrhi"
    NAME "H_resource_state"
    DRIVER_SPECIFIC_NAME "HD_directx12_resource_state"
    DRIVER_UPPER_CASE_NAME "DIRECTX_12"
    TARGET_HPP_FILE_PATH "${NRHI_RESOURCE_STATE_HPP_FILE_PATH}"
    TARGET_CPP_FILE_PATH "${NRHI_RESOURCE_STATE_CPP_FILE_PATH}"
    INCLUDES
        "<nrhi/directx12/resource_state_helper.hpp>"
    ADDITIONAL_CODE
        ""
    VALUES
        "is_writable" "b8(ED_resource_state)"
)

message(STATUS "<NRHI::Drivers::DirectX12> Implement resource state helper done")