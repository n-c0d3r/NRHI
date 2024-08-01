
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
        "clear_state" "void(
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
		"bind_descriptor_heaps" "void(
			TKPA_valid<A_command_list> command_list_p,
			const TG_span<TK_valid<A_descriptor_heap>>& descriptor_heap_p_span
		)"
		"bind_pipeline_state" "void(
			TKPA_valid<A_command_list> command_list_p,
			TKPA_valid<A_pipeline_state> pipeline_state_p
		)"
		"ZC_bind_pipeline_state" "void(
			TKPA_valid<A_command_list> command_list_p,
			KPA_valid_compute_pipeline_state_handle pipeline_state_p
		)"
		"ZC_bind_root_signature" "void(
			TKPA_valid<A_command_list> command_list_p,
			TKPA_valid<A_root_signature> root_signature_p
		)"
		"ZC_bind_root_descriptor_table" "void(
			TKPA_valid<A_command_list> command_list_p,
			u32 root_param_index,
			F_descriptor_gpu_address gpu_address
		)"
		"ZC_bind_root_constants" "void(
			TKPA_valid<A_command_list> command_list_p,
			u32 root_param_index,
			const TG_span<u32>& constant_span,
			u32 offset_in_constants
		)"
		"ZC_bind_root_constant" "void(
			TKPA_valid<A_command_list> command_list_p,
			u32 root_param_index,
			u32 root_constant,
			u32 offset_in_constants
		)"
		"ZC_bind_root_srv" "void(
			TKPA_valid<A_command_list> command_list_p,
			u32 root_param_index,
			KPA_valid_srv_handle srv_p
		)"
		"ZC_bind_root_uav" "void(
			TKPA_valid<A_command_list> command_list_p,
			u32 root_param_index,
			KPA_valid_uav_handle uav_p
		)"
		"ZC_bind_root_cbv_with_resource" "void(
			TKPA_valid<A_command_list> command_list_p,
			u32 root_param_index,
			TKPA_valid<A_resource> resource_p
		)"
		"ZC_bind_root_srv_with_resource" "void(
			TKPA_valid<A_command_list> command_list_p,
			u32 root_param_index,
			TKPA_valid<A_resource> resource_p
		)"
		"ZC_bind_root_uav_with_resource" "void(
			TKPA_valid<A_command_list> command_list_p,
			u32 root_param_index,
			TKPA_valid<A_resource> resource_p
		)"
		"ZC_bind_root_cbv_with_gpu_virtual_address" "void(
			TKPA_valid<A_command_list> command_list_p,
			u32 root_param_index,
			F_resource_gpu_virtual_address gpu_virtual_address
		)"
		"ZC_bind_root_srv_with_gpu_virtual_address" "void(
			TKPA_valid<A_command_list> command_list_p,
			u32 root_param_index,
			F_resource_gpu_virtual_address gpu_virtual_address
		)"
		"ZC_bind_root_uav_with_gpu_virtual_address" "void(
			TKPA_valid<A_command_list> command_list_p,
			u32 root_param_index,
			F_resource_gpu_virtual_address gpu_virtual_address
		)"
		"ZG_bind_pipeline_state" "void(
			TKPA_valid<A_command_list> command_list_p,
			KPA_valid_graphics_pipeline_state_handle pipeline_state_p
		)"
		"ZG_bind_root_signature" "void(
			TKPA_valid<A_command_list> command_list_p,
			TKPA_valid<A_root_signature> root_signature_p
		)"
		"ZG_bind_root_descriptor_table" "void(
			TKPA_valid<A_command_list> command_list_p,
			u32 root_param_index,
			F_descriptor_gpu_address gpu_address
		)"
		"ZG_bind_root_constants" "void(
			TKPA_valid<A_command_list> command_list_p,
			u32 root_param_index,
			const TG_span<u32>& constant_span,
			u32 offset_in_constants
		)"
		"ZG_bind_root_constant" "void(
			TKPA_valid<A_command_list> command_list_p,
			u32 root_param_index,
			u32 root_constant,
			u32 offset_in_constants
		)"
		"ZG_bind_root_srv" "void(
			TKPA_valid<A_command_list> command_list_p,
			u32 root_param_index,
			KPA_valid_srv_handle srv_p
		)"
		"ZG_bind_root_uav" "void(
			TKPA_valid<A_command_list> command_list_p,
			u32 root_param_index,
			KPA_valid_uav_handle uav_p
		)"
		"ZG_bind_root_cbv_with_resource" "void(
			TKPA_valid<A_command_list> command_list_p,
			u32 root_param_index,
			TKPA_valid<A_resource> resource_p
		)"
		"ZG_bind_root_srv_with_resource" "void(
			TKPA_valid<A_command_list> command_list_p,
			u32 root_param_index,
			TKPA_valid<A_resource> resource_p
		)"
		"ZG_bind_root_uav_with_resource" "void(
			TKPA_valid<A_command_list> command_list_p,
			u32 root_param_index,
			TKPA_valid<A_resource> resource_p
		)"
		"ZG_bind_root_cbv_with_gpu_virtual_address" "void(
			TKPA_valid<A_command_list> command_list_p,
			u32 root_param_index,
			F_resource_gpu_virtual_address gpu_virtual_address
		)"
		"ZG_bind_root_srv_with_gpu_virtual_address" "void(
			TKPA_valid<A_command_list> command_list_p,
			u32 root_param_index,
			F_resource_gpu_virtual_address gpu_virtual_address
		)"
		"ZG_bind_root_uav_with_gpu_virtual_address" "void(
			TKPA_valid<A_command_list> command_list_p,
			u32 root_param_index,
			F_resource_gpu_virtual_address gpu_virtual_address
		)"
		"ZOM_bind_frame_buffer" "void(
			TKPA_valid<A_command_list> command_list_p,
			TKPA_valid<A_frame_buffer> frame_buffer_p
		)"
		"ZIA_bind_index_buffer" "void(
			TKPA_valid<A_command_list> command_list_p,
			KPA_valid_buffer_handle index_buffer_p,
			u32 offset
		)"
		"ZIA_bind_input_buffers" "void(
			TKPA_valid<A_command_list> command_list_p,
			const TG_span<K_valid_buffer_handle>& input_buffer_p_span,
			const TG_span<u32>& offset_span,
			u32 base_slot_index
		)"
		"ZIA_bind_input_buffer" "void(
			TKPA_valid<A_command_list> command_list_p,
			KPA_valid_buffer_handle input_buffer_p,
			u32 offset,
			u32 slot_index
		)"
		"ZIA_bind_index_buffer_with_gpu_virtual_address" "void(
			TKPA_valid<A_command_list> command_list_p,
			F_resource_gpu_virtual_address gpu_virtual_address,
			u32 size,
			ED_format format
		)"
		"ZIA_bind_input_buffers_with_gpu_virtual_address" "void(
			TKPA_valid<A_command_list> command_list_p,
			TG_span<F_resource_gpu_virtual_address> gpu_virtual_addresses,
			TG_span<u32> sizes,
			TG_span<u32> strides,
			u32 base_slot_index
		)"
		"ZIA_bind_input_buffer_with_gpu_virtual_address" "void(
			TKPA_valid<A_command_list> command_list_p,
			F_resource_gpu_virtual_address gpu_virtual_address,
			u32 size,
			u32 stride,
			u32 slot_index
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
		)"
		"async_draw" "void(
			TKPA_valid<A_command_list> command_list_p,
			u32 vertex_count,
			u32 base_vertex_location
		)"
		"async_draw_instanced" "void(
            TKPA_valid<A_command_list> command_list_p,
            u32 vertex_count_per_instance,
            u32 instance_count,
            u32 base_vertex_location,
            u32 base_instance_location
        )"
		"async_draw_indexed" "void(
            TKPA_valid<A_command_list> command_list_p,
            u32 index_count,
            u32 base_index_location,
            u32 base_vertex_location
        )"
		"async_draw_indexed_instanced" "void(
            TKPA_valid<A_command_list> command_list_p,
			u32 index_count_per_instance,
			u32 instance_count,
			u32 base_index_location,
			u32 base_vertex_location,
			u32 base_instance_location
        )"
		"async_dispatch" "void(
            TKPA_valid<A_command_list> command_list_p,
            PA_vector3_u32 thread_group_count_3d
        )"
		"async_execute_indirect" "void(
			TKPA_valid<A_command_list> command_list_p,
			TKPA_valid<A_command_signature> command_signature_p,
			u32 max_command_count,
			KPA_buffer_handle argument_buffer_p,
			u64 argument_buffer_offset_in_bytes,
			KPA_buffer_handle count_buffer_p,
			u64 count_buffer_offset_in_bytes
		)"
)

message(STATUS "<NRHI::Drivers::DirectX12> Implement command list done")