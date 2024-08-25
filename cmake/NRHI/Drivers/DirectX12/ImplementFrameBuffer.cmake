
message(STATUS "<NRHI::Drivers::DirectX12> Start implementing frame buffer")

include(NRHI/Utilities/FunctionHelper)



NCPP_SetGlobal(NRHI_FRAME_BUFFER_HPP_FILE_PATH "${NRHI_GENERATED_FILES_DIR}/frame_buffer.hpp")
NCPP_SetGlobal(NRHI_FRAME_BUFFER_CPP_FILE_PATH "${NRHI_GENERATED_FILES_DIR}/frame_buffer.cpp")



NRHI_FunctionHelper_CreateFunctionClass(
    NAMESPACE "nrhi"
    NAME "H_frame_buffer"
    DRIVER_SPECIFIC_NAME "HD_directx12_frame_buffer"
    DRIVER_UPPER_CASE_NAME "DIRECTX_12"
    TARGET_HPP_FILE_PATH "${NRHI_FRAME_BUFFER_HPP_FILE_PATH}"
    TARGET_CPP_FILE_PATH "${NRHI_FRAME_BUFFER_CPP_FILE_PATH}"
    INCLUDES
        "<nrhi/directx12/frame_buffer.hpp>"
    ADDITIONAL_CODE
        "namespace nrhi {
        }"
    VALUES
        "create" "TU<A_frame_buffer>(
            TKPA_valid<A_device> device_p,
            const F_frame_buffer_desc& desc
        )"
        "create_with_unmanaged_descriptor_cpu_addresses" "TU<A_frame_buffer>(
			TKPA_valid<A_device> device_p,
			TG_fixed_vector<F_descriptor_cpu_address, 8, false> color_attachment_descriptor_cpu_addresses,
			F_descriptor_cpu_address depth_stencil_attachment_descriptor_cpu_address
		)"
)

message(STATUS "<NRHI::Drivers::DirectX12> Implement frame buffer done")