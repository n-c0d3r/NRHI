
message(STATUS "<NRHI::Drivers::DirectX12> Start implementing command list")

include(NRHI/Utilities/FunctionHelper)



NCPP_SetGlobal(NRHI_COMMAND_LIST_HPP_FILE_PATH "${NRHI_GENERATED_FILES_DIR}/command_list.hpp")
NCPP_SetGlobal(NRHI_COMMAND_LIST_CPP_FILE_PATH "${NRHI_GENERATED_FILES_DIR}/command_list.cpp")



NRHI_FunctionHelper_CreateFunctionClass(
    NAMESPACE "nrhi"
    NAME "H_command_list"
    DRIVER_SPECIFIC_NAME "HD_directx12_command_list"
    DRIVER_UPPER_CASE_NAME "DIRECTX_12"
    TARGET_HPP_FILE_PATH "${NRHI_COMMAND_LIST_HPP_FILE_PATH}"
    TARGET_CPP_FILE_PATH "${NRHI_COMMAND_LIST_CPP_FILE_PATH}"
    INCLUDES
        "<nrhi/directx12/command_list.hpp>"
    ADDITIONAL_CODE
        "namespace nrhi {
            class A_device;
        }"
    VALUES
        "create_with_command_allocator" "TU<A_command_list>(TKPA_valid<A_device> device_p, const F_command_list_desc& desc)"
        "async_begin" "void(
            TKPA_valid<A_command_list> command_list_p,
            TKPA_valid<A_command_allocator> command_allocator_p
        )"
        "async_end" "void(
            TKPA_valid<A_command_list> command_list_p
        )"
        "async_resource_barrier" "void(
            TKPA_valid<A_command_list> command_list_p,
            const F_resource_barrier& resource_barrier
        )"
        "async_resource_barriers" "void(
            TKPA_valid<A_command_list> command_list_p,
            const TG_span<F_resource_barrier>& resource_barriers
        )"
        "async_clear_state" "void(
            TKPA_valid<A_command_list> command_list_p
        )"
		"async_clear_rtv" "void(
            TKPA_valid<A_command_list> command_list_p,
            K_valid_rtv_handle rtv_p,
            PA_vector4_f32 color
        )"
		"async_clear_dsv" "void(
            TKPA_valid<A_command_list> command_list_p,
            K_valid_dsv_handle dsv_p,
            ED_clear_flag flag,
            f32 depth,
            u8 stencil
        )"
        "async_clear_rtv_with_descriptor" "void(
            TKPA_valid<A_command_list> command_list_p,
            F_descriptor_cpu_address rtv_cpu_address,
            PA_vector4_f32 color
        )"
        "async_clear_dsv_with_descriptor" "void(
            TKPA_valid<A_command_list> command_list_p,
            F_descriptor_cpu_address dsv_cpu_address,
            ED_clear_flag flag,
            f32 depth,
            u8 stencil
        )"
		"ZOM_async_bind_frame_buffer" "void(
			TKPA_valid<A_command_list> command_list_p,
			TKPA_valid<A_frame_buffer> frame_buffer_p
		)"
        "async_copy_resource" "void(
			TKPA_valid<A_command_list> command_list_p,
			TKPA_valid<A_resource> dst_resource_p,
			TKPA_valid<A_resource> src_resource_p
		)"
		"async_copy_buffer_region" "void(
			TKPA_valid<A_command_list> command_list_p,
			TKPA_valid<A_resource> dst_resource_p,
			TKPA_valid<A_resource> src_resource_p,
			u64 dst_offset,
			u64 src_offset,
			u64 size
		);"
)

message(STATUS "<NRHI::Drivers::DirectX12> Implement command list done")