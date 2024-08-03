
message(STATUS "<NRHI::Drivers::Alternatives> Start implementing device")

include(NRHI/Utilities/AlternativeHelper)



NCPP_SetGlobal(NRHI_DEVICE_HPP_FILE_PATH "${NRHI_GENERATED_FILES_DIR}/device.hpp")
NCPP_SetGlobal(NRHI_DEVICE_CPP_FILE_PATH "${NRHI_GENERATED_FILES_DIR}/device.cpp")



NRHI_AlternativeHelper_CreateAlternative(
    NAMESPACE "nrhi"
    NAME "H_device"
    MAIN_HPP_FILE_PATH "${NRHI_DEVICE_HPP_FILE_PATH}"
    MAIN_CPP_FILE_PATH "${NRHI_DEVICE_CPP_FILE_PATH}"
    INCLUDES
        "<nrhi/device_base.hpp>"
    ADDITIONAL_CODE
        "namespace nrhi {
            class A_adapter;
        }"
    VALUES
        "create" "TU<A_device>(TKPA_valid<A_adapter> adapter_p)"
        "max_queue_count" "u32(A_device*)"
)

message(STATUS "<NRHI::Drivers::Alternatives> Implement device done")