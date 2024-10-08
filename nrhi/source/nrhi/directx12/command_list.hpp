#pragma once

/** @file nrhi/directx12/command_list.hpp
*
*   Implement directx12 command_list.
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
#include <nrhi/resource_barrier.hpp>
#include <nrhi/texture_copy_location.hpp>

#pragma endregion



namespace nrhi {

	struct F_directx12_command_list_create_with_command_allocator_flag {};



    class NRHI_API F_directx12_command_list : public A_command_list {

	public:
		friend class HD_directx12_command_list;



	private:
		ID3D12GraphicsCommandList* d3d12_command_list_p_ = 0;

		NCPP_ENABLE_IF_ASSERTION_ENABLED(
			b8 is_in_record_ = true;
		);

	public:
		NCPP_FORCE_INLINE ID3D12GraphicsCommandList* d3d12_command_list_p() const noexcept { return d3d12_command_list_p_; }
    	NCPP_FORCE_INLINE void set_d3d12_command_list_p_unsafe(ID3D12GraphicsCommandList* value) noexcept { d3d12_command_list_p_ = value; }

#ifdef NRHI_ENABLE_DRIVER_DEBUGGER
    	virtual void set_debug_name(const F_debug_name& value) override;
#endif



    public:
        F_directx12_command_list(
			TKPA_valid<A_device> device_p,
			const F_command_list_desc& desc,
			F_directx12_command_list_create_with_command_allocator_flag
		);
        ~F_directx12_command_list();

    };



	class NRHI_API HD_directx12_command_list {

#ifdef NRHI_ENABLE_DRIVER_DEBUGGER
	public:
		static void begin_event(
			TKPA_valid<A_command_list> command_list_p,
			PA_vector3_f32 color,
			const F_debug_name& name
		);
		static void end_event(TKPA_valid<A_command_list> command_list_p);
#endif

    public:
        static TU<A_command_list> create_with_command_allocator(TKPA_valid<A_device> device_p, const F_command_list_desc& desc);

	public:
		static void async_begin(
			TKPA_valid<A_command_list> command_list_p,
			TKPA_valid<A_command_allocator> command_allocator_p
		);
		static void async_end(TKPA_valid<A_command_list> command_list_p);

	public:
		static void async_resource_barrier(
			TKPA_valid<A_command_list> command_list_p,
			const F_resource_barrier& resource_barrier
		);
		static void async_resource_barriers(
			TKPA_valid<A_command_list> command_list_p,
			const TG_span<F_resource_barrier>& resource_barriers
		);

	public:
		static void clear_state(
			TKPA_valid<A_command_list> command_list_p
		);
		static void async_clear_rtv(
			TKPA_valid<A_command_list> command_list_p,
			K_valid_rtv_handle rtv_p,
			PA_vector4_f32 color
		);
		static void async_clear_dsv(
			TKPA_valid<A_command_list> command_list_p,
			K_valid_dsv_handle dsv_p,
			ED_clear_flag flag,
			f32 depth,
			u8 stencil
		);
		static void async_clear_rtv_with_descriptor(
			TKPA_valid<A_command_list> command_list_p,
			F_descriptor_cpu_address rtv_cpu_address,
			PA_vector4_f32 color
		);
		static void async_clear_dsv_with_descriptor(
			TKPA_valid<A_command_list> command_list_p,
			F_descriptor_cpu_address dsv_cpu_address,
			ED_clear_flag flag,
			f32 depth,
			u8 stencil
		);
		static void async_clear_uav(
            TKPA_valid<A_command_list> command_list_p,
            F_descriptor_gpu_address uav_gpu_address,
            F_descriptor_cpu_address uav_cpu_address,
            TKPA_valid<A_resource> resource_p,
            u32 values[4]
        );

	public:
		static void bind_descriptor_heaps(
			TKPA_valid<A_command_list> command_list_p,
			const TG_span<TK_valid<A_descriptor_heap>>& descriptor_heap_p_span
		);

	public:
		static void bind_pipeline_state(
			TKPA_valid<A_command_list> command_list_p,
			TKPA_valid<A_pipeline_state> pipeline_state_p
		);

	public:
		static void ZC_bind_pipeline_state(
			TKPA_valid<A_command_list> command_list_p,
			KPA_valid_compute_pipeline_state_handle pipeline_state_p
		);
		static void ZC_bind_root_signature(
			TKPA_valid<A_command_list> command_list_p,
			TKPA_valid<A_root_signature> root_signature_p
		);
		static void ZC_bind_root_descriptor_table(
			TKPA_valid<A_command_list> command_list_p,
			u32 root_param_index,
			F_descriptor_gpu_address gpu_address
		);
		static void ZC_bind_root_constants(
			TKPA_valid<A_command_list> command_list_p,
			u32 root_param_index,
			const TG_span<u32>& constant_span,
			u32 offset_in_constants
		);
		static void ZC_bind_root_constant(
			TKPA_valid<A_command_list> command_list_p,
			u32 root_param_index,
			u32 root_constant,
			u32 offset_in_constants
		);
		static void ZC_bind_root_srv(
			TKPA_valid<A_command_list> command_list_p,
			u32 root_param_index,
			KPA_valid_srv_handle srv_p
		);
		static void ZC_bind_root_uav(
			TKPA_valid<A_command_list> command_list_p,
			u32 root_param_index,
			KPA_valid_uav_handle uav_p
		);
		static void ZC_bind_root_cbv_with_resource(
			TKPA_valid<A_command_list> command_list_p,
			u32 root_param_index,
			TKPA_valid<A_resource> resource_p
		);
		static void ZC_bind_root_srv_with_resource(
			TKPA_valid<A_command_list> command_list_p,
			u32 root_param_index,
			TKPA_valid<A_resource> resource_p
		);
		static void ZC_bind_root_uav_with_resource(
			TKPA_valid<A_command_list> command_list_p,
			u32 root_param_index,
			TKPA_valid<A_resource> resource_p
		);
		static void ZC_bind_root_cbv_with_gpu_virtual_address(
			TKPA_valid<A_command_list> command_list_p,
			u32 root_param_index,
			F_resource_gpu_virtual_address gpu_virtual_address
		);
		static void ZC_bind_root_srv_with_gpu_virtual_address(
			TKPA_valid<A_command_list> command_list_p,
			u32 root_param_index,
			F_resource_gpu_virtual_address gpu_virtual_address
		);
		static void ZC_bind_root_uav_with_gpu_virtual_address(
			TKPA_valid<A_command_list> command_list_p,
			u32 root_param_index,
			F_resource_gpu_virtual_address gpu_virtual_address
		);

	public:
		static void ZG_bind_pipeline_state(
			TKPA_valid<A_command_list> command_list_p,
			KPA_valid_graphics_pipeline_state_handle pipeline_state_p
		);
		static void ZG_bind_root_signature(
			TKPA_valid<A_command_list> command_list_p,
			TKPA_valid<A_root_signature> root_signature_p
		);
		static void ZG_bind_root_descriptor_table(
			TKPA_valid<A_command_list> command_list_p,
			u32 root_param_index,
			F_descriptor_gpu_address gpu_address
		);
		static void ZG_bind_root_constants(
			TKPA_valid<A_command_list> command_list_p,
			u32 root_param_index,
			const TG_span<u32>& constant_span,
			u32 offset_in_constants
		);
		static void ZG_bind_root_constant(
			TKPA_valid<A_command_list> command_list_p,
			u32 root_param_index,
			u32 root_constant,
			u32 offset_in_constants
		);
		static void ZG_bind_root_srv(
			TKPA_valid<A_command_list> command_list_p,
			u32 root_param_index,
			KPA_valid_srv_handle srv_p
		);
		static void ZG_bind_root_uav(
			TKPA_valid<A_command_list> command_list_p,
			u32 root_param_index,
			KPA_valid_uav_handle uav_p
		);
		static void ZG_bind_root_cbv_with_resource(
			TKPA_valid<A_command_list> command_list_p,
			u32 root_param_index,
			TKPA_valid<A_resource> resource_p
		);
		static void ZG_bind_root_srv_with_resource(
			TKPA_valid<A_command_list> command_list_p,
			u32 root_param_index,
			TKPA_valid<A_resource> resource_p
		);
		static void ZG_bind_root_uav_with_resource(
			TKPA_valid<A_command_list> command_list_p,
			u32 root_param_index,
			TKPA_valid<A_resource> resource_p
		);
		static void ZG_bind_root_cbv_with_gpu_virtual_address(
			TKPA_valid<A_command_list> command_list_p,
			u32 root_param_index,
			F_resource_gpu_virtual_address gpu_virtual_address
		);
		static void ZG_bind_root_srv_with_gpu_virtual_address(
			TKPA_valid<A_command_list> command_list_p,
			u32 root_param_index,
			F_resource_gpu_virtual_address gpu_virtual_address
		);
		static void ZG_bind_root_uav_with_gpu_virtual_address(
			TKPA_valid<A_command_list> command_list_p,
			u32 root_param_index,
			F_resource_gpu_virtual_address gpu_virtual_address
		);

    public:
    	static void ZRS_bind_viewport(
			TKPA_valid<A_command_list> command_list_p,
			const F_viewport& viewport
		);

	public:
		static void ZOM_bind_frame_buffer(
			TKPA_valid<A_command_list> command_list_p,
			TKPA_valid<A_frame_buffer> frame_buffer_p
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
		static void ZIA_bind_index_buffer_with_gpu_virtual_address(
			TKPA_valid<A_command_list> command_list_p,
			F_resource_gpu_virtual_address gpu_virtual_address,
			u32 size,
			ED_format format
		);
		static void ZIA_bind_input_buffers_with_gpu_virtual_address(
			TKPA_valid<A_command_list> command_list_p,
			TG_span<F_resource_gpu_virtual_address> gpu_virtual_addresses,
			TG_span<u32> sizes,
			TG_span<u32> strides,
			u32 base_slot_index
		);
		static void ZIA_bind_input_buffer_with_gpu_virtual_address(
			TKPA_valid<A_command_list> command_list_p,
			F_resource_gpu_virtual_address gpu_virtual_address,
			u32 size,
			u32 stride,
			u32 slot_index
		);

	public:
		static void async_copy_resource(
			TKPA_valid<A_command_list> command_list_p,
			TKPA_valid<A_resource> dst_resource_p,
			TKPA_valid<A_resource> src_resource_p
		);
		static void async_copy_buffer_region(
			TKPA_valid<A_command_list> command_list_p,
			TKPA_valid<A_resource> dst_resource_p,
			TKPA_valid<A_resource> src_resource_p,
			u64 dst_offset,
			u64 src_offset,
			u64 size
		);
    	static void async_copy_texture_region(
			TKPA_valid<A_command_list> command_list_p,
			const F_texture_copy_location& dst_location,
			const F_texture_copy_location& src_location,
			PA_vector3_u32 dst_coord,
			PA_vector3_u32 src_coord,
			PA_vector3_u32 volume
		);

	public:
		static void async_draw(
			TKPA_valid<A_command_list> command_list_p,
			u32 vertex_count,
			u32 base_vertex_location
		);
		static void async_draw_instanced(
			TKPA_valid<A_command_list> command_list_p,
			u32 vertex_count_per_instance,
			u32 instance_count,
			u32 base_vertex_location,
			u32 base_instance_location
		);
		static void async_draw_indexed(
			TKPA_valid<A_command_list> command_list_p,
			u32 index_count,
			u32 base_index_location,
			u32 base_vertex_location
		);
		static void async_draw_indexed_instanced(
			TKPA_valid<A_command_list> command_list_p,
			u32 index_count_per_instance,
			u32 instance_count,
			u32 base_index_location,
			u32 base_vertex_location,
			u32 base_instance_location
		);

	public:
		static void async_dispatch(
			TKPA_valid<A_command_list> command_list_p,
			PA_vector3_u32 thread_group_count_3d
		);

	public:
		static void async_dispatch_mesh(
			TKPA_valid<A_command_list> command_list_p,
			PA_vector3_u32 thread_group_count_3d
		);

	public:
		static void async_execute_indirect(
			TKPA_valid<A_command_list> command_list_p,
			TKPA_valid<A_command_signature> command_signature_p,
			u32 command_count,
			KPA_valid_buffer_handle argument_buffer_p,
			u64 argument_buffer_offset_in_bytes
		);
		static void async_execute_indirect_with_dynamic_count(
			TKPA_valid<A_command_list> command_list_p,
			TKPA_valid<A_command_signature> command_signature_p,
			u32 max_command_count,
			KPA_valid_buffer_handle argument_buffer_p,
			u64 argument_buffer_offset_in_bytes,
			KPA_valid_buffer_handle count_buffer_p,
			u64 count_buffer_offset_in_bytes
		);



#pragma region Alternative Functions
#ifdef NRHI_DRIVER_ENABLE_INTERFACE_ONLY_SUPPORTS
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
			KPA_valid_buffer_handle buffer_p,
			u32 buffer_offset
		);
		static void draw_indexed_instanced_indirect(
			TKPA_valid<A_command_list> command_list_p,
			KPA_valid_buffer_handle buffer_p,
			u32 buffer_offset
		);

	public:
		static void dispatch_indirect(
			TKPA_valid<A_command_list> command_list_p,
			KPA_valid_buffer_handle buffer_p,
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
#endif // NRHI_DRIVER_ENABLE_INTERFACE_ONLY_SUPPORTS
#pragma endregion

    };

}
