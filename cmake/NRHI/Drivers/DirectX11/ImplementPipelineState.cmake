
message(STATUS "<NRHI::Drivers::DirectX11> Start implementing pipeline state")

include(NRHI/Utilities/FunctionHelper)



NCPP_SetGlobal(NRHI_PIPELINE_STATE_HPP_FILE_PATH "${NRHI_GENERATED_FILES_DIR}/pipeline_state.hpp")
NCPP_SetGlobal(NRHI_PIPELINE_STATE_CPP_FILE_PATH "${NRHI_GENERATED_FILES_DIR}/pipeline_state.cpp")



NRHI_FunctionHelper_CreateFunctionClass(
    NAMESPACE "nrhi"
    NAME "H_pipeline_state"
    DRIVER_SPECIFIC_NAME "HD_directx11_pipeline_state"
    DRIVER_UPPER_CASE_NAME "DIRECTX_11"
    TARGET_HPP_FILE_PATH "${NRHI_PIPELINE_STATE_HPP_FILE_PATH}"
    TARGET_CPP_FILE_PATH "${NRHI_PIPELINE_STATE_CPP_FILE_PATH}"
    INCLUDES
        "<nrhi/directx11/pipeline_state.hpp>"
    ADDITIONAL_CODE
        "namespace nrhi {
        }"
    VALUES
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
        "graphics_options" "const F_graphics_pipeline_state_options&(
			KPA_valid_graphics_pipeline_state_handle pso_p
		)"
		"compute_options" "const F_compute_pipeline_state_options&(
			KPA_valid_compute_pipeline_state_handle pso_p
		)"
)

message(STATUS "<NRHI::Drivers::DirectX11> Implement pipeline state done")