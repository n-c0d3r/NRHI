
message(STATUS "<NRHI::Drivers::DirectX11> Start implementing sampler state")

include(NRHI/Utilities/FunctionHelper)



NCPP_SetGlobal(NRHI_SAMPLER_STATE_HPP_FILE_PATH "${NRHI_GENERATED_FILES_DIR}/sampler_state.hpp")
NCPP_SetGlobal(NRHI_SAMPLER_STATE_CPP_FILE_PATH "${NRHI_GENERATED_FILES_DIR}/sampler_state.cpp")



NRHI_FunctionHelper_CreateFunctionClass(
    NAMESPACE "nrhi"
    NAME "H_sampler_state"
    DRIVER_SPECIFIC_NAME "HD_directx11_sampler_state"
    DRIVER_UPPER_CASE_NAME "DIRECTX_11"
    TARGET_HPP_FILE_PATH "${NRHI_SAMPLER_STATE_HPP_FILE_PATH}"
    TARGET_CPP_FILE_PATH "${NRHI_SAMPLER_STATE_CPP_FILE_PATH}"
    INCLUDES
        "<nrhi/directx11/sampler_state.hpp>"
    ADDITIONAL_CODE
        ""
    VALUES
        "create" "TU<A_sampler_state>(
            TKPA_valid<A_device> device_p,
            const F_sampler_state_desc& desc
        )"
)

message(STATUS "<NRHI::Drivers::DirectX11> Implement sampler state done")