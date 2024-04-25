
message(STATUS "<NRHI::Drivers::DirectX11> Start implementing frame buffer")

include(NRHI/Utilities/FunctionHelper)



NCPP_SetGlobal(NRHI_FRAME_BUFFER_HPP_FILE_PATH "${NRHI_GENERATED_FILES_DIR}/frame_buffer.hpp")
NCPP_SetGlobal(NRHI_FRAME_BUFFER_CPP_FILE_PATH "${NRHI_GENERATED_FILES_DIR}/frame_buffer.cpp")



NRHI_FunctionHelper_CreateFunctionClass(
    NAMESPACE "nrhi"
    NAME "H_frame_buffer"
    DRIVER_SPECIFIC_NAME "HD_directx11_frame_buffer"
    DRIVER_UPPER_CASE_NAME "DIRECTX_11"
    TARGET_HPP_FILE_PATH "${NRHI_FRAME_BUFFER_HPP_FILE_PATH}"
    TARGET_CPP_FILE_PATH "${NRHI_FRAME_BUFFER_CPP_FILE_PATH}"
    INCLUDES
        "<nrhi/directx11/frame_buffer.hpp>"
    ADDITIONAL_CODE
        "namespace nrhi {
        }"
    VALUES
        "create" "TU<A_frame_buffer>(
            TKPA_valid<A_device> device_p,
            const F_frame_buffer_desc& desc
        )"
)

message(STATUS "<NRHI::Drivers::DirectX11> Implement frame buffer done")