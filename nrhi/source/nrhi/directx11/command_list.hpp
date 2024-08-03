#pragma once

/** @file nrhi/directx11/command_list.hpp
*
*   Implement directx11 command_list.
*/



//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



#pragma region Includes

////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////

#include <nrhi/prerequisites.hpp>

////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////

#include <nrhi/command_list_base.hpp>
#include <nrhi/pipeline_state_base.hpp>
#include <nrhi/graphics_pipeline_state.hpp>
#include <nrhi/compute_pipeline_state.hpp>
#include <nrhi/buffer_handle.hpp>
#include <nrhi/clear_flag.hpp>
#include <nrhi/resource_view_handle.hpp>

#pragma endregion



namespace nrhi {

    class A_device;
    class A_pipeline_state;
    class A_frame_buffer;
    class A_sampler_state;

    class F_directx11_graphics_pipeline_state;



    class NRHI_API F_directx11_command_list : public A_command_list {

	public:
		friend class HD_directx11_command_list;



    private:
        ID3D11DeviceContext* d3d11_device_context_p_ = 0;

		NCPP_ENABLE_IF_ASSERTION_ENABLED(
			b8 is_in_record_ = true;
		);

    public:
        NCPP_FORCE_INLINE ID3D11DeviceContext* d3d11_device_context_p() noexcept { return d3d11_device_context_p_; }
		NCPP_FORCE_INLINE void set_d3d11_device_context_p_unsafe(ID3D11DeviceContext* value) noexcept { d3d11_device_context_p_ = value; }



    public:
        F_directx11_command_list(TKPA_valid<A_device> device_p, const F_command_list_desc& desc);
        ~F_directx11_command_list();

    };



    class NRHI_API HD_directx11_command_list {

    public:
        static TU<A_command_list> create(TKPA_valid<A_device> device_p, const F_command_list_desc& desc);

	public:
		static void begin(
			TKPA_valid<A_command_list> command_list_p
		);
		static void end(
			TKPA_valid<A_command_list> command_list_p
		);

	public:
		static void clear_state(
			TKPA_valid<A_command_list> command_list_p
		);
		static void clear_rtv(
			TKPA_valid<A_command_list> command_list_p,
			KPA_valid_rtv_handle rtv_p,
			PA_vector4_f32 color
		);
		static void clear_dsv(
			TKPA_valid<A_command_list> command_list_p,
			KPA_valid_dsv_handle dsv_p,
			ED_clear_flag flag,
			f32 depth,
			u8 stencil
		);

	public:
		static void bind_pipeline_state(
			TKPA_valid<A_command_list> command_list_p,
			TKPA_valid<A_pipeline_state> pipeline_state_p
		);
		static void ZG_bind_pipeline_state(
			TKPA_valid<A_command_list> command_list_p,
			KPA_valid_graphics_pipeline_state_handle graphics_pipeline_state_p
		);
		static void ZC_bind_pipeline_state(
			TKPA_valid<A_command_list> command_list_p,
			KPA_valid_compute_pipeline_state_handle compute_pipeline_state_p
		);

	public:
		static void ZIA_bind_index_buffer(
			TKPA_valid<A_command_list> command_list_p,
			KPA_valid_buffer_handle index_buffer_p,
			u32 offset
		);
		static void ZIA_bind_input_buffers(
			TKPA_valid<A_command_list> command_list_p,
			const TG_span<K_valid_buffer_handle>& input_buffer_p_span,
			const TG_span<u32>& offset_span,
			u32 base_slot_index
		);
		static void ZIA_bind_input_buffer(
			TKPA_valid<A_command_list> command_list_p,
			KPA_valid_buffer_handle input_buffer_p,
			u32 offset,
			u32 slot_index
		);

	public:
		static void ZVS_bind_constant_buffers(
			TKPA_valid<A_command_list> command_list_p,
			const TG_span<K_valid_buffer_handle>& constant_buffer_p_span,
			u32 base_slot_index
		);
		static void ZVS_bind_constant_buffer(
			TKPA_valid<A_command_list> command_list_p,
			KPA_valid_buffer_handle constant_buffer_p,
			u32 slot_index
		);
		static void ZVS_bind_srvs(
			TKPA_valid<A_command_list> command_list_p,
			const TG_span<K_valid_srv_handle>& srv_p_span,
			u32 base_slot_index
		);
		static void ZVS_bind_srv(
			TKPA_valid<A_command_list> command_list_p,
			KPA_valid_srv_handle srv_p,
			u32 slot_index
		);
		static void ZVS_bind_sampler_states(
			TKPA_valid<A_command_list> command_list_p,
			const TG_span<TK_valid<A_sampler_state>>& sampler_state_p_span,
			u32 base_slot_index
		);
		static void ZVS_bind_sampler_state(
			TKPA_valid<A_command_list> command_list_p,
			TKPA_valid<A_sampler_state> sampler_state_p,
			u32 slot_index
		);

	public:
		static void ZPS_bind_constant_buffers(
			TKPA_valid<A_command_list> command_list_p,
			const TG_span<K_valid_buffer_handle>& constant_buffer_p_span,
			u32 base_slot_index
		);
		static void ZPS_bind_constant_buffer(
			TKPA_valid<A_command_list> command_list_p,
			KPA_valid_buffer_handle constant_buffer_p,
			u32 slot_index
		);
		static void ZPS_bind_srvs(
			TKPA_valid<A_command_list> command_list_p,
			const TG_span<K_valid_srv_handle>& srv_p_span,
			u32 base_slot_index
		);
		static void ZPS_bind_srv(
			TKPA_valid<A_command_list> command_list_p,
			KPA_valid_srv_handle srv_p,
			u32 slot_index
		);
		static void ZPS_bind_sampler_states(
			TKPA_valid<A_command_list> command_list_p,
			const TG_span<TK_valid<A_sampler_state>>& sampler_state_p_span,
			u32 base_slot_index
		);
		static void ZPS_bind_sampler_state(
			TKPA_valid<A_command_list> command_list_p,
			TKPA_valid<A_sampler_state> sampler_state_p,
			u32 slot_index
		);

	public:
		static void ZOM_bind_frame_buffer(
			TKPA_valid<A_command_list> command_list_p,
			TKPA_valid<A_frame_buffer> frame_buffer_p
		);

	public:
		static void ZCS_bind_constant_buffers(
			TKPA_valid<A_command_list> command_list_p,
			const TG_span<K_valid_buffer_handle>& constant_buffer_p_span,
			u32 base_slot_index
		);
		static void ZCS_bind_constant_buffer(
			TKPA_valid<A_command_list> command_list_p,
			KPA_valid_buffer_handle constant_buffer_p,
			u32 slot_index
		);
		static void ZCS_bind_srvs(
			TKPA_valid<A_command_list> command_list_p,
			const TG_span<K_valid_srv_handle>& srv_p_span,
			u32 base_slot_index
		);
		static void ZCS_bind_srv(
			TKPA_valid<A_command_list> command_list_p,
			KPA_valid_srv_handle srv_p,
			u32 slot_index
		);
		static void ZCS_bind_uavs(
			TKPA_valid<A_command_list> command_list_p,
			const TG_span<K_valid_uav_handle>& uav_p_span,
			u32 base_slot_index
		);
		static void ZCS_bind_uav(
			TKPA_valid<A_command_list> command_list_p,
			KPA_valid_uav_handle uav_p,
			u32 slot_index
		);
		static void ZCS_bind_sampler_states(
			TKPA_valid<A_command_list> command_list_p,
			const TG_span<TK_valid<A_sampler_state>>& sampler_state_p_span,
			u32 base_slot_index
		);
		static void ZCS_bind_sampler_state(
			TKPA_valid<A_command_list> command_list_p,
			TKPA_valid<A_sampler_state> sampler_state_p,
			u32 slot_index
		);

	public:
		static void draw(
			TKPA_valid<A_command_list> command_list_p,
			u32 vertex_count,
			u32 base_vertex_location
		);
		static void draw_instanced(
			TKPA_valid<A_command_list> command_list_p,
			u32 vertex_count_per_instance,
			u32 instance_count,
			u32 base_vertex_location,
			u32 base_instance_location
		);
		static void draw_indexed(
			TKPA_valid<A_command_list> command_list_p,
			u32 index_count,
			u32 base_index_location,
			u32 base_vertex_location
		);
		static void draw_indexed_instanced(
			TKPA_valid<A_command_list> command_list_p,
			u32 index_count_per_instance,
			u32 instance_count,
			u32 base_index_location,
			u32 base_vertex_location,
			u32 base_instance_location
		);

	public:
		static void dispatch(
			TKPA_valid<A_command_list> command_list_p,
			PA_vector3_u32 thread_group_count_3d
		);

	public:
		static void draw_instanced_indirect(
			TKPA_valid<A_command_list> command_list_p,
			KPA_buffer_handle buffer_p,
			u32 buffer_offset
		);
		static void draw_indexed_instanced_indirect(
			TKPA_valid<A_command_list> command_list_p,
			KPA_buffer_handle buffer_p,
			u32 buffer_offset
		);

	public:
		static void dispatch_indirect(
			TKPA_valid<A_command_list> command_list_p,
			KPA_buffer_handle buffer_p,
			u32 buffer_offset
		);

	public:
		static void update_resource_data(
			TKPA_valid<A_command_list> command_list_p,
			TKPA_valid<A_resource> resource_p,
			void* data_p,
			u32 data_size,
			u32 src_data_offset,
			u32 dst_data_offset
		);

	public:
		static void generate_mips(
			TKPA_valid<A_command_list> command_list_p,
			KPA_valid_srv_handle srv_p
		);

    };

}
