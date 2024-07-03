
message(STATUS "<NRHI::Drivers::DirectX11> Start implementing swapchain")

include(NRHI/Utilities/FunctionHelper)



NCPP_SetGlobal(NRHI_SWAPCHAIN_HPP_FILE_PATH "${NRHI_GENERATED_FILES_DIR}/swapchain.hpp")
NCPP_SetGlobal(NRHI_SWAPCHAIN_CPP_FILE_PATH "${NRHI_GENERATED_FILES_DIR}/swapchain.cpp")



NRHI_FunctionHelper_CreateFunctionClass(
    NAMESPACE "nrhi"
    NAME "H_swapchain"
    DRIVER_SPECIFIC_NAME "HD_directx11_swapchain"
    DRIVER_UPPER_CASE_NAME "DIRECTX_11"
    TARGET_HPP_FILE_PATH "${NRHI_SWAPCHAIN_HPP_FILE_PATH}"
    TARGET_CPP_FILE_PATH "${NRHI_SWAPCHAIN_CPP_FILE_PATH}"
    INCLUDES
        "<nrhi/directx11/swapchain.hpp>"
    ADDITIONAL_CODE
        "namespace nrhi {
            class A_command_queue;
        }"
    VALUES
        "create" "TU<A_swapchain>(
            TKPA_valid<A_command_queue> command_queue_p,
            TKPA_valid<F_surface> surface_p,
            const F_swapchain_desc& desc
        )"
        "present" "void(TKPA_valid<A_swapchain> swapchain_p)"
)

message(STATUS "<NRHI::Drivers::DirectX11> Implement swapchain done")