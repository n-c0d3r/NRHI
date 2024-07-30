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

#pragma endregion



namespace nrhi {

	struct F_directx12_command_list_create_with_command_allocator_flag {};



	struct F_directx12_temp_command_list_state {

		NCPP_ENABLE_IF_ASSERTION_ENABLED(
			TK<A_pipeline_state> pipeline_state_p;
		);

		u32 vertex_buffer_count = 0;
		u32 instance_buffer_count = 0;

		D3D12_VERTEX_BUFFER_VIEW d3d12_instance_buffer_views[NRHI_MAX_INSTANCE_BUFFER_COUNT_PER_DRAWCALL];
		NCPP_ENABLE_IF_ASSERTION_ENABLED(
			std::array<b8, NRHI_MAX_VERTEX_BUFFER_COUNT_PER_DRAWCALL> is_vertex_buffer_binded_array;
			std::array<b8, NRHI_MAX_INSTANCE_BUFFER_COUNT_PER_DRAWCALL> is_instance_buffer_binded_array;
		);

		NCPP_ENABLE_IF_ASSERTION_ENABLED(
			b8 is_index_buffer_binded = false;
		);

	};



    class NRHI_API F_directx12_command_list : public A_command_list {

	public:
		friend class HD_directx12_command_list;



	private:
		ID3D12GraphicsCommandList* d3d12_command_list_p_ = 0;

		F_directx12_temp_command_list_state temp_state_;

	public:
		NCPP_FORCE_INLINE ID3D12GraphicsCommandList* d3d12_command_list_p() const noexcept { return d3d12_command_list_p_; }
		NCPP_FORCE_INLINE void set_d3d12_command_list_p_unsafe(ID3D12GraphicsCommandList* value) noexcept { d3d12_command_list_p_ = value; }



    public:
        F_directx12_command_list(
			TKPA_valid<A_device> device_p,
			const F_command_list_desc& desc,
			F_directx12_command_list_create_with_command_allocator_flag
		);
        ~F_directx12_command_list();

    };



    class NRHI_API HD_directx12_command_list {

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
		static void ZIA_bind_vertex_buffers(
			TKPA_valid<A_command_list> command_list_p,
			const TG_span<K_valid_buffer_handle>& vertex_buffer_p_span,
			const TG_span<u32>& offset_span,
			u32 base_slot_index
		);
		static void ZIA_bind_vertex_buffer(
			TKPA_valid<A_command_list> command_list_p,
			KPA_valid_buffer_handle vertex_buffer_p,
			u32 offset,
			u32 slot_index
		);
		static void ZIA_bind_instance_buffers(
			TKPA_valid<A_command_list> command_list_p,
			const TG_span<K_valid_buffer_handle>& instance_buffer_p_span,
			const TG_span<u32>& offset_span,
			u32 base_slot_index
		);
		static void ZIA_bind_instance_buffer(
			TKPA_valid<A_command_list> command_list_p,
			KPA_valid_buffer_handle instance_buffer_p,
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
		static void ZIA_bind_vertex_buffers_with_gpu_virtual_address(
			TKPA_valid<A_command_list> command_list_p,
			TG_span<F_resource_gpu_virtual_address> gpu_virtual_addresses,
			TG_span<u32> sizes,
			TG_span<u32> strides,
			u32 base_slot_index
		);
		static void ZIA_bind_vertex_buffer_with_gpu_virtual_address(
			TKPA_valid<A_command_list> command_list_p,
			F_resource_gpu_virtual_address gpu_virtual_address,
			u32 size,
			u32 stride,
			u32 slot_index
		);
		static void ZIA_bind_instance_buffers_with_gpu_virtual_address(
			TKPA_valid<A_command_list> command_list_p,
			TG_span<F_resource_gpu_virtual_address> gpu_virtual_addresses,
			TG_span<u32> sizes,
			TG_span<u32> strides,
			u32 base_slot_index
		);
		static void ZIA_bind_instance_buffer_with_gpu_virtual_address(
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
		static void async_draw_instanced_indirect(
			TKPA_valid<A_command_list> command_list_p,
			KPA_indirect_buffer_handle indirect_buffer_p,
			u32 indirect_buffer_offset
		);
		static void async_draw_indexed_instanced_indirect(
			TKPA_valid<A_command_list> command_list_p,
			KPA_indirect_buffer_handle indirect_buffer_p,
			u32 indirect_buffer_offset
		);

	public:
		static void async_dispatch_indirect(
			TKPA_valid<A_command_list> command_list_p,
			KPA_indirect_buffer_handle indirect_buffer_p,
			u32 indirect_buffer_offset
		);

	private:
		static void temp_state_apply_instance_buffers(
			const F_directx12_temp_command_list_state& temp_state,
			ID3D12GraphicsCommandList* d3d12_command_list_p
		);

    };

}
