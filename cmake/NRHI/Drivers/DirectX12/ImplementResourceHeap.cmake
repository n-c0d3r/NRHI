
message(STATUS "<NRHI::Drivers::DirectX12> Start implementing resource heap")

include(NRHI/Utilities/FunctionHelper)



NCPP_SetGlobal(NRHI_RESOURCE_HEAP_HPP_FILE_PATH "${NRHI_GENERATED_FILES_DIR}/resource_heap.hpp")
NCPP_SetGlobal(NRHI_RESOURCE_HEAP_CPP_FILE_PATH "${NRHI_GENERATED_FILES_DIR}/resource_heap.cpp")



NRHI_FunctionHelper_CreateFunctionClass(
    NAMESPACE "nrhi"
    NAME "H_resource_heap"
    DRIVER_SPECIFIC_NAME "HD_directx12_resource_heap"
    DRIVER_UPPER_CASE_NAME "DIRECTX_12"
    TARGET_HPP_FILE_PATH "${NRHI_RESOURCE_HEAP_HPP_FILE_PATH}"
    TARGET_CPP_FILE_PATH "${NRHI_RESOURCE_HEAP_CPP_FILE_PATH}"
    INCLUDES
        "<nrhi/directx12/resource_heap.hpp>"
    ADDITIONAL_CODE
        "namespace nrhi {
        }"
    VALUES
        "create" "TU<A_resource_heap>(
            TKPA_valid<A_device> device_p,
            const F_resource_heap_desc& desc
        )"
)

message(STATUS "<NRHI::Drivers::DirectX12> Implement resource heap done")