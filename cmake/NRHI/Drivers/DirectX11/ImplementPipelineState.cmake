
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
            TK_valid<A_device> device_p,
            const F_pipeline_state_desc& desc
        )"
        "create_graphics_pipeline_state" "U_graphics_pipeline_state_handle(
            TK_valid<A_device> device_p,
            const F_pipeline_state_desc& desc
        )"
)

message(STATUS "<NRHI::Drivers::DirectX11> Implement pipeline state done")