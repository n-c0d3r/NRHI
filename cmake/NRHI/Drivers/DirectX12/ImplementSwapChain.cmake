
message(STATUS "<NRHI::Drivers::DirectX12> Start implementing swapchain")

include(NRHI/Utilities/FunctionHelper)



NCPP_SetGlobal(NRHI_SWAPCHAIN_HPP_FILE_PATH "${NRHI_GENERATED_FILES_DIR}/swapchain.hpp")
NCPP_SetGlobal(NRHI_SWAPCHAIN_CPP_FILE_PATH "${NRHI_GENERATED_FILES_DIR}/swapchain.cpp")



NRHI_FunctionHelper_CreateFunctionClass(
    NAMESPACE "nrhi"
    NAME "H_swapchain"
    DRIVER_SPECIFIC_NAME "HD_directx12_swapchain"
    DRIVER_UPPER_CASE_NAME "DIRECTX_12"
    TARGET_HPP_FILE_PATH "${NRHI_SWAPCHAIN_HPP_FILE_PATH}"
    TARGET_CPP_FILE_PATH "${NRHI_SWAPCHAIN_CPP_FILE_PATH}"
    INCLUDES
        "<nrhi/directx12/swapchain.hpp>"
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
        "current_rtv_index" "u8(TKPA_valid<A_swapchain> swapchain_p)"
        "rtv_p" "K_valid_rtv_handle(TKPA_valid<A_swapchain> swapchain_p, u8 index)"
        "update_back_rtv" "void(TKPA_valid<A_swapchain> swapchain_p)"
        "back_rtv_p" "K_valid_rtv_handle(TKPA_valid<A_swapchain> swapchain_p)"
        "back_buffer_p" "K_valid_texture_2d_handle(TKPA_valid<A_swapchain> swapchain_p)"
        "async_present" "void(TKPA_valid<A_swapchain> swapchain_p)"

    ALTERNATIVE_VALUES
        "present" "void(TKPA_valid<A_swapchain> swapchain_p)"
)

message(STATUS "<NRHI::Drivers::DirectX12> Implement swapchain done")