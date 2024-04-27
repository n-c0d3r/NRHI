
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
			PA_vector4 color
		)"
        "clear_dsv" "void(
            TKPA_valid<A_command_list> command_list_p,
            KPA_valid_dsv_handle dsv_p,
            E_clear_flag flag,
            f32 depth,
            u8 stencil
        )"
        "bind_graphics_pipeline_state" "void(
            TKPA_valid<A_command_list> command_list_p,
            KPA_valid_graphics_pipeline_state_handle graphics_pipeline_state_p
        )"
        "bind_compute_pipeline_state" "void(
            TKPA_valid<A_command_list> command_list_p,
            KPA_valid_compute_pipeline_state_handle compute_pipeline_state_p
        )"
        "ZIA_bind_index_buffer" "void(
            TKPA_valid<A_command_list> command_list_p,
            KPA_valid_buffer_handle index_buffer_p,
            u32 offset
        )"
        "ZIA_bind_vertex_buffers" "void(
            TKPA_valid<A_command_list> command_list_p,
            const TG_span<K_valid_buffer_handle>& vertex_buffer_p_span,
            const TG_span<u32>& offset_span,
			u32 base_slot_index
        )"
        "ZIA_bind_vertex_buffer" "void(
            TKPA_valid<A_command_list> command_list_p,
            KPA_valid_buffer_handle vertex_buffer_p,
            u32> offset,
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
            u32> offset,
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
        "ZOM_bind_frame_buffer" "void(
            TKPA_valid<A_command_list> command_list_p,
            TKPA_valid<A_frame_buffer> frame_buffer_p
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
)

message(STATUS "<NRHI::Drivers::DirectX11> Implement command_list done")