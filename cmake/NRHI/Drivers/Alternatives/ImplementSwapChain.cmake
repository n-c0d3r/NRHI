
message(STATUS "<NRHI::Drivers::Alternatives> Start implementing swapchain")

include(NRHI/Utilities/AlternativeHelper)



NCPP_SetGlobal(NRHI_SWAPCHAIN_HPP_FILE_PATH "${NRHI_GENERATED_FILES_DIR}/swapchain.hpp")
NCPP_SetGlobal(NRHI_SWAPCHAIN_CPP_FILE_PATH "${NRHI_GENERATED_FILES_DIR}/swapchain.cpp")



NRHI_AlternativeHelper_CreateAlternative(
    NAMESPACE "nrhi"
    NAME "H_swapchain"
    MAIN_HPP_FILE_PATH "${NRHI_SWAPCHAIN_HPP_FILE_PATH}"
    MAIN_CPP_FILE_PATH "${NRHI_SWAPCHAIN_CPP_FILE_PATH}"
    INCLUDES
        "<nrhi/swapchain_base.hpp>"
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
        "back_rtv_p" "K_valid_rtv_handle(TKPA_valid<A_swapchain> swapchain_p)"
        "present" "void(TKPA_valid<A_swapchain> swapchain_p)"
)

message(STATUS "<NRHI::Drivers::Alternatives> Implement swapchain done")