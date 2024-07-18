
message(STATUS "<NRHI::Drivers::DirectX12> Start implementing descriptor heap")

include(NRHI/Utilities/FunctionHelper)



NCPP_SetGlobal(NRHI_DESCRIPTOR_HEAP_HPP_FILE_PATH "${NRHI_GENERATED_FILES_DIR}/descriptor_heap.hpp")
NCPP_SetGlobal(NRHI_DESCRIPTOR_HEAP_CPP_FILE_PATH "${NRHI_GENERATED_FILES_DIR}/descriptor_heap.cpp")



NRHI_FunctionHelper_CreateFunctionClass(
    NAMESPACE "nrhi"
    NAME "H_descriptor_heap"
    DRIVER_SPECIFIC_NAME "HD_directx12_descriptor_heap"
    DRIVER_UPPER_CASE_NAME "DIRECTX_12"
    TARGET_HPP_FILE_PATH "${NRHI_DESCRIPTOR_HEAP_HPP_FILE_PATH}"
    TARGET_CPP_FILE_PATH "${NRHI_DESCRIPTOR_HEAP_CPP_FILE_PATH}"
    INCLUDES
        "<nrhi/directx12/descriptor_heap.hpp>"
    ADDITIONAL_CODE
        "namespace nrhi {
            class A_device;
        }"
    VALUES
        "create" "TU<A_descriptor_heap>(
            TKPA_valid<A_device> device_p,
            const F_descriptor_heap_desc& desc
        )"
)

message(STATUS "<NRHI::Drivers::DirectX12> Implement descriptor heap done")