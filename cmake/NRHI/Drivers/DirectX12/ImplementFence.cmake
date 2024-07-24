
message(STATUS "<NRHI::Drivers::DirectX12> Start implementing fence")

include(NRHI/Utilities/FunctionHelper)



NCPP_SetGlobal(NRHI_FENCE_HPP_FILE_PATH "${NRHI_GENERATED_FILES_DIR}/fence.hpp")
NCPP_SetGlobal(NRHI_FENCE_CPP_FILE_PATH "${NRHI_GENERATED_FILES_DIR}/fence.cpp")



NRHI_FunctionHelper_CreateFunctionClass(
    NAMESPACE "nrhi"
    NAME "H_fence"
    DRIVER_SPECIFIC_NAME "HD_directx12_fence"
    DRIVER_UPPER_CASE_NAME "DIRECTX_12"
    TARGET_HPP_FILE_PATH "${NRHI_FENCE_HPP_FILE_PATH}"
    TARGET_CPP_FILE_PATH "${NRHI_FENCE_CPP_FILE_PATH}"
    INCLUDES
        "<nrhi/directx12/fence.hpp>"
    ADDITIONAL_CODE
        "namespace nrhi {
            class A_device;
        }"
    VALUES
        "create" "TU<A_fence>(
            TKPA_valid<A_device> device_p,
            const F_fence_desc& desc
        )"
        "value" "u64(TKPA_valid<A_fence> fence_p)"
        "wait" "void(TKPA_valid<A_fence> fence_p, u64 target_value)"
        "is_completed" "b8(TKPA_valid<A_fence> fence_p, u64 target_value)"
)

message(STATUS "<NRHI::Drivers::DirectX12> Implement fence done")