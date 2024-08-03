
message(STATUS "<NRHI::Drivers::DirectX12> Start implementing pipeline state")

include(NRHI/Utilities/FunctionHelper)



NCPP_SetGlobal(NRHI_PIPELINE_STATE_HPP_FILE_PATH "${NRHI_GENERATED_FILES_DIR}/pipeline_state.hpp")
NCPP_SetGlobal(NRHI_PIPELINE_STATE_CPP_FILE_PATH "${NRHI_GENERATED_FILES_DIR}/pipeline_state.cpp")



NRHI_FunctionHelper_CreateFunctionClass(
    NAMESPACE "nrhi"
    NAME "H_pipeline_state"
    DRIVER_SPECIFIC_NAME "HD_directx12_pipeline_state"
    DRIVER_UPPER_CASE_NAME "DIRECTX_12"
    TARGET_HPP_FILE_PATH "${NRHI_PIPELINE_STATE_HPP_FILE_PATH}"
    TARGET_CPP_FILE_PATH "${NRHI_PIPELINE_STATE_CPP_FILE_PATH}"
    INCLUDES
        "<nrhi/directx12/pipeline_state.hpp>"
    ADDITIONAL_CODE
        "namespace nrhi {
        }"
    VALUES
        "create_with_root_signature" "TU<A_pipeline_state>(
            TKPA_valid<A_device> device_p,
            const A_pipeline_state_desc& desc,
		    TKPA_valid<A_root_signature> root_signature_p
        )"
        "create_graphics_pipeline_state_with_root_signature" "U_graphics_pipeline_state_handle(
            TKPA_valid<A_device> device_p,
            const F_graphics_pipeline_state_options& options,
		    TKPA_valid<A_root_signature> root_signature_p
        )"
        "create_compute_pipeline_state_with_root_signature" "U_compute_pipeline_state_handle(
			TKPA_valid<A_device> device_p,
			const F_compute_pipeline_state_options& options,
		    TKPA_valid<A_root_signature> root_signature_p
		)"
        "graphics_options" "const F_graphics_pipeline_state_options&(
			KPA_valid_graphics_pipeline_state_handle pso_p
		)"
        "compute_options" "const F_compute_pipeline_state_options&(
			KPA_valid_compute_pipeline_state_handle pso_p
		)"

    ALTERNATIVE_VALUES
        "create" "TU<A_pipeline_state>(
            TKPA_valid<A_device> device_p,
            const A_pipeline_state_desc& desc
        )"
        "create_graphics_pipeline_state" "U_graphics_pipeline_state_handle(
            TKPA_valid<A_device> device_p,
            const F_graphics_pipeline_state_options& options
        )"
        "create_compute_pipeline_state" "U_compute_pipeline_state_handle(
			TKPA_valid<A_device> device_p,
			const F_compute_pipeline_state_options& options
		)"
)

message(STATUS "<NRHI::Drivers::DirectX12> Implement pipeline state done")