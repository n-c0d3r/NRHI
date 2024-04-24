
message(STATUS "<NRHI::Drivers::DirectX11> Start implementing pipeline")

include(NRHI/Utilities/FunctionHelper)



NCPP_SetGlobal(NRHI_PIPELINE_HPP_FILE_PATH "${NRHI_GENERATED_FILES_DIR}/pipeline.hpp")
NCPP_SetGlobal(NRHI_PIPELINE_CPP_FILE_PATH "${NRHI_GENERATED_FILES_DIR}/pipeline.cpp")



NRHI_FunctionHelper_CreateFunctionClass(
    NAMESPACE "nrhi"
    NAME "H_pipeline"
    DRIVER_SPECIFIC_NAME "HD_directx11_pipeline"
    DRIVER_UPPER_CASE_NAME "DIRECTX_11"
    TARGET_HPP_FILE_PATH "${NRHI_PIPELINE_HPP_FILE_PATH}"
    TARGET_CPP_FILE_PATH "${NRHI_PIPELINE_CPP_FILE_PATH}"
    INCLUDES
        "<nrhi/directx11/pipeline.hpp>"
    ADDITIONAL_CODE
        "namespace nrhi {
        }"
    VALUES
        "create" "TU<A_pipeline>(
            TK_valid<A_device> device_p,
            const F_pipeline_desc& desc
        )"
        "create_graphics_pipeline" "U_graphics_pipeline_handle(
            TK_valid<A_device> device_p,
            const F_pipeline_desc& desc
        )"
)

message(STATUS "<NRHI::Drivers::DirectX11> Implement pipeline done")