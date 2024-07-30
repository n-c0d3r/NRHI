
message(STATUS "<NRHI::Drivers::DirectX11> Start implementing command_list")

include(NRHI/Utilities/FunctionHelper)



NCPP_SetGlobal(NRHI_COMMAND_LIST_HPP_FILE_PATH "${NRHI_GENERATED_FILES_DIR}/command_list.hpp")
NCPP_SetGlobal(NRHI_COMMAND_LIST_CPP_FILE_PATH "${NRHI_GENERATED_FILES_DIR}/command_list.cpp")



NRHI_FunctionHelper_CreateFunctionClass(
    NAMESPACE "nrhi"
    NAME "H_command_list"
    DRIVER_SPECIFIC_NAME "HD_directx11_command_list"
    DRIVER_UPPER_CASE_NAME "DIRECTX_11"
    TARGET_HPP_FILE_PATH "${NRHI_COMMAND_LIST_HPP_FILE_PATH}"
    TARGET_CPP_FILE_PATH "${NRHI_COMMAND_LIST_CPP_FILE_PATH}"
    INCLUDES
        "<nrhi/directx11/command_list.hpp>"
    ADDITIONAL_CODE
        "namespace nrhi {
            class A_device;
        }"
    VALUES
        "create" "TU<A_command_list>(TKPA_valid<A_device> device_p, const F_command_list_desc& desc)"
        "clear_state" "void(
            TKPA_valid<A_command_list> command_list_p
        )"
        "clear_rtv" "void(
			TKPA_valid<A_command_list> command_list_p,
			KPA_valid_rtv_handle rtv_p,
			PA_vector4_f32 color
		)"
        "clear_dsv" "void(
            TKPA_valid<A_command_list> command_list_p,
            KPA_valid_dsv_handle dsv_p,
            ED_clear_flag flag,
            f32 depth,
            u8 stencil
        )"
        "bind_pipeline_state" "void(
            TKPA_valid<A_command_list> command_list_p,
            TKPA_valid<A_pipeline_state> pipeline_state_p
        )"
        "ZG_bind_pipeline_state" "void(
            TKPA_valid<A_command_list> command_list_p,
            KPA_valid_graphics_pipeline_state_handle graphics_pipeline_state_p
        )"
        "ZC_bind_pipeline_state" "void(
            TKPA_valid<A_command_list> command_list_p,
            KPA_valid_compute_pipeline_state_handle compute_pipeline_state_p
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
        "ZIA_bind_instance_buffers" "void(
            TKPA_valid<A_command_list> command_list_p,
            const TG_span<K_valid_buffer_handle>& instance_buffer_p_span,
            const TG_span<u32>& offset_span,
			u32 base_slot_index
        )"
        "ZIA_bind_instance_buffer" "void(
            TKPA_valid<A_command_list> command_list_p,
            KPA_valid_buffer_handle instance_buffer_p,
            u32 offset,
            u32 slot_index
        )"
        "ZVS_bind_constant_buffers" "void(
            TKPA_valid<A_command_list> command_list_p,
            const TG_span<K_valid_buffer_handle>& constant_buffer_p_span,
            u32 base_slot_index
        )"
        "ZVS_bind_constant_buffer" "void(
            TKPA_valid<A_command_list> command_list_p,
            KPA_valid_buffer_handle constant_buffer_p,
            u32 slot_index
        )"
        "ZVS_bind_srvs" "void(
            TKPA_valid<A_command_list> command_list_p,
            const TG_span<K_valid_srv_handle>& srv_p_span,
            u32 base_slot_index
        )"
        "ZVS_bind_srv" "void(
            TKPA_valid<A_command_list> command_list_p,
            KPA_valid_srv_handle srv_p,
            u32 slot_index
        )"
        "ZVS_bind_sampler_states" "void(
            TKPA_valid<A_command_list> command_list_p,
            const TG_span<TK_valid<A_sampler_state>>& sampler_state_p_span,
            u32 base_slot_index
        )"
        "ZVS_bind_sampler_state" "void(
            TKPA_valid<A_command_list> command_list_p,
            TKPA_valid<A_sampler_state> sampler_state_p,
            u32 slot_index
        )"
        "ZPS_bind_constant_buffers" "void(
            TKPA_valid<A_command_list> command_list_p,
            const TG_span<K_valid_buffer_handle>& constant_buffer_p_span,
            u32 base_slot_index
        )"
        "ZPS_bind_constant_buffer" "void(
            TKPA_valid<A_command_list> command_list_p,
            KPA_valid_buffer_handle constant_buffer_p,
            u32 slot_index
        )"
        "ZPS_bind_srvs" "void(
            TKPA_valid<A_command_list> command_list_p,
            const TG_span<K_valid_srv_handle>& srv_p_span,
            u32 base_slot_index
        )"
        "ZPS_bind_srv" "void(
            TKPA_valid<A_command_list> command_list_p,
            KPA_valid_srv_handle srv_p,
            u32 slot_index
        )"
        "ZPS_bind_sampler_states" "void(
            TKPA_valid<A_command_list> command_list_p,
            const TG_span<TK_valid<A_sampler_state>>& sampler_state_p_span,
            u32 base_slot_index
        )"
        "ZPS_bind_sampler_state" "void(
            TKPA_valid<A_command_list> command_list_p,
            TKPA_valid<A_sampler_state> sampler_state_p,
            u32 slot_index
        )"
        "ZOM_bind_frame_buffer" "void(
            TKPA_valid<A_command_list> command_list_p,
            TKPA_valid<A_frame_buffer> frame_buffer_p
        )"
        "ZCS_bind_constant_buffers" "void(
            TKPA_valid<A_command_list> command_list_p,
            const TG_span<K_valid_buffer_handle>& constant_buffer_p_span,
            u32 base_slot_index
        )"
        "ZCS_bind_constant_buffer" "void(
            TKPA_valid<A_command_list> command_list_p,
            KPA_valid_buffer_handle constant_buffer_p,
            u32 slot_index
        )"
        "ZCS_bind_srvs" "void(
            TKPA_valid<A_command_list> command_list_p,
            const TG_span<K_valid_srv_handle>& srv_p_span,
            u32 base_slot_index
        )"
        "ZCS_bind_srv" "void(
            TKPA_valid<A_command_list> command_list_p,
            KPA_valid_srv_handle srv_p,
            u32 slot_index
        )"
        "ZCS_bind_uavs" "void(
            TKPA_valid<A_command_list> command_list_p,
            const TG_span<K_valid_uav_handle>& uav_p_span,
            u32 base_slot_index
        )"
        "ZCS_bind_uav" "void(
            TKPA_valid<A_command_list> command_list_p,
            KPA_valid_uav_handle uav_p,
            u32 slot_index
        )"
        "ZCS_bind_sampler_states" "void(
            TKPA_valid<A_command_list> command_list_p,
            const TG_span<TK_valid<A_sampler_state>>& sampler_state_p_span,
            u32 base_slot_index
        )"
        "ZCS_bind_sampler_state" "void(
            TKPA_valid<A_command_list> command_list_p,
            TKPA_valid<A_sampler_state> sampler_state_p,
            u32 slot_index
        )"
        "draw" "void(
            TKPA_valid<A_command_list> command_list_p,
            u32 vertex_count,
            u32 base_vertex_location
        )"
        "draw_instanced" "void(
            TKPA_valid<A_command_list> command_list_p,
            u32 vertex_count_per_instance,
            u32 instance_count,
            u32 base_vertex_location,
            u32 base_instance_location
        )"
        "draw_indexed" "void(
            TKPA_valid<A_command_list> command_list_p,
            u32 index_count,
            u32 base_index_location,
            u32 base_vertex_location
        )"
        "draw_indexed_instanced" "void(
            TKPA_valid<A_command_list> command_list_p,
			u32 index_count_per_instance,
			u32 instance_count,
			u32 base_index_location,
			u32 base_vertex_location,
			u32 base_instance_location
        )"
        "dispatch" "void(
            TKPA_valid<A_command_list> command_list_p,
            PA_vector3_u32 thread_group_count_3d
        )"
        "draw_instanced_indirect" "void(
            TKPA_valid<A_command_list> command_list_p,
            KPA_indirect_buffer_handle indirect_buffer_p,
            u32 indirect_buffer_offset
        )"
        "draw_indexed_instanced_indirect" "void(
            TKPA_valid<A_command_list> command_list_p,
            KPA_indirect_buffer_handle indirect_buffer_p,
            u32 indirect_buffer_offset
        )"
        "dispatch_indirect" "void(
            TKPA_valid<A_command_list> command_list_p,
            KPA_indirect_buffer_handle indirect_buffer_p,
            u32 indirect_buffer_offset
        )"
        "update_resource_data" "void(
			TKPA_valid<A_command_list> command_list_p,
			TKPA_valid<A_resource> resource_p,
			void* data_p,
            u32 data_size,
            u32 src_data_offset,
            u32 dst_data_offset
		)"
        "generate_mips" "void(
            TKPA_valid<A_command_list> command_list_p,
            KPA_valid_srv_handle srv_p
        )"
)

message(STATUS "<NRHI::Drivers::DirectX11> Implement command_list done")