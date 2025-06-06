
message(STATUS "<NRHI::Drivers::DirectX12> Start implementing sampler state")

include(NRHI/Utilities/FunctionHelper)



NCPP_SetGlobal(NRHI_SAMPLER_STATE_HPP_FILE_PATH "${NRHI_GENERATED_FILES_DIR}/sampler_state.hpp")
NCPP_SetGlobal(NRHI_SAMPLER_STATE_CPP_FILE_PATH "${NRHI_GENERATED_FILES_DIR}/sampler_state.cpp")



NRHI_FunctionHelper_CreateFunctionClass(
    NAMESPACE "nrhi"
    NAME "H_sampler_state"
    DRIVER_SPECIFIC_NAME "HD_directx12_sampler_state"
    DRIVER_UPPER_CASE_NAME "DIRECTX_12"
    TARGET_HPP_FILE_PATH "${NRHI_SAMPLER_STATE_HPP_FILE_PATH}"
    TARGET_CPP_FILE_PATH "${NRHI_SAMPLER_STATE_CPP_FILE_PATH}"
    INCLUDES
        "<nrhi/directx12/sampler_state.hpp>"
    ADDITIONAL_CODE
        ""
    VALUES
        "create_with_managed_descriptor" "TU<A_sampler_state>(
            TKPA_valid<A_device> device_p,
            const F_sampler_state_desc& desc,
            const F_descriptor& managed_descriptor
        )"
        "create_with_unmanaged_descriptor_handle" "TU<A_sampler_state>(
            TKPA_valid<A_device> device_p,
            const F_descriptor_handle& unmanaged_descriptor_handle
        )"
    ALTERNATIVE_VALUES
        "create" "TU<A_sampler_state>(
            TKPA_valid<A_device> device_p,
            const F_sampler_state_desc& desc
        )"
)

message(STATUS "<NRHI::Drivers::DirectX12> Implement sampler state done")