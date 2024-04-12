
message(STATUS "<NRHI::Drivers::DirectX11> Start implementing device")

include(NRHI/Utilities/FunctionHelper)



NCPP_SetGlobal(NRHI_DEVICE_HPP_FILE_PATH "${NRHI_GENERATED_FILES_DIR}/device.hpp")
NCPP_SetGlobal(NRHI_DEVICE_CPP_FILE_PATH "${NRHI_GENERATED_FILES_DIR}/device.cpp")



NRHI_FunctionHelper_CreateFunctionClass(
    NAMESPACE "nrhi"
    NAME "H_device"
    DRIVER_SPECIFIC_NAME "HD_directx11_device"
    DRIVER_UPPER_CASE_NAME "DIRECTX_11"
    TARGET_HPP_FILE_PATH "${NRHI_DEVICE_HPP_FILE_PATH}"
    TARGET_CPP_FILE_PATH "${NRHI_DEVICE_CPP_FILE_PATH}"
    INCLUDES
        "<nrhi/directx11/device.hpp>"
    ADDITIONAL_CODE
        "namespace nrhi {
            class A_adapter;
        }"
    VALUES
        "create" "TU<A_device>(TK_valid<A_adapter> adapter_p)"
        "max_queue_count" "void(A_device*)"
)

message(STATUS "<NRHI::Drivers::DirectX11> Implement device done")