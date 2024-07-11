
message(STATUS "<NRHI::Drivers::DirectX12> Start implementing driver")

include(NRHI/Utilities/FunctionHelper)



NCPP_SetGlobal(NRHI_DRIVER_HPP_FILE_PATH "${NRHI_GENERATED_FILES_DIR}/driver.hpp")
NCPP_SetGlobal(NRHI_DRIVER_CPP_FILE_PATH "${NRHI_GENERATED_FILES_DIR}/driver.cpp")



NRHI_FunctionHelper_CreateFunctionClass(
    NAMESPACE "nrhi"
    NAME "H_driver"
    DRIVER_SPECIFIC_NAME "HD_directx12_driver"
    DRIVER_UPPER_CASE_NAME "DIRECTX_12"
    TARGET_HPP_FILE_PATH "${NRHI_DRIVER_HPP_FILE_PATH}"
    TARGET_CPP_FILE_PATH "${NRHI_DRIVER_CPP_FILE_PATH}"
    INCLUDES
        "<nrhi/directx12/driver.hpp>"
    ADDITIONAL_CODE
        ""
    VALUES
        "is_support_descriptor_management" "bool()"
        "is_support_advanced_resource" "bool()"
)

message(STATUS "<NRHI::Drivers::DirectX12> Implement driver done")