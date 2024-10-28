#pragma once

/** @file nrhi/command_list.external_use_only.inl
*
*   Implement command list inline functions that is only used by external.
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
#include <nrhi/command_list.hpp>

#pragma endregion



namespace nrhi {

#ifdef NRHI_DRIVER_SUPPORT_SIMPLE_WORK_SUBMISSION
	NCPP_FORCE_INLINE void A_command_list::begin() {

		H_command_list::begin(NCPP_KTHIS());
	}
	NCPP_FORCE_INLINE void A_command_list::end() {
		H_command_list::end(NCPP_KTHIS());
	}
#endif // NRHI_DRIVER_SUPPORT_SIMPLE_WORK_SUBMISSION

	NCPP_FORCE_INLINE void A_command_list::clear_state() {

		H_command_list::clear_state(NCPP_KTHIS());
	}
#ifdef NRHI_DRIVER_SUPPORT_SIMPLE_WORK_SUBMISSION
	NCPP_FORCE_INLINE void A_command_list::clear_rtv(KPA_valid_rtv_handle rtv_p, PA_vector4 color) {

		H_command_list::clear_rtv(NCPP_KTHIS(), rtv_p, color);
	}
	NCPP_FORCE_INLINE void A_command_list::clear_dsv(
		KPA_valid_dsv_handle dsv_p,
		ED_clear_flag flag,
		f32 depth,
		u8 stencil
	) {
		H_command_list::clear_dsv(
			NCPP_KTHIS(),
			dsv_p,
			flag,
			depth,
			stencil
		);
	}
#endif // NRHI_DRIVER_SUPPORT_SIMPLE_WORK_SUBMISSION

	NCPP_FORCE_INLINE void A_command_list::bind_pipeline_state(TKPA_valid<A_pipeline_state> pipeline_state_p) {

		H_command_list::bind_pipeline_state(NCPP_KTHIS(), pipeline_state_p);
	}
	NCPP_FORCE_INLINE void A_command_list::ZG_bind_pipeline_state(KPA_valid_graphics_pipeline_state_handle graphics_pipeline_state_p) {

		H_command_list::ZG_bind_pipeline_state(NCPP_KTHIS(), graphics_pipeline_state_p);
	}
	NCPP_FORCE_INLINE void A_command_list::ZC_bind_pipeline_state(KPA_valid_compute_pipeline_state_handle compute_pipeline_state_p) {

		H_command_list::ZC_bind_pipeline_state(NCPP_KTHIS(), compute_pipeline_state_p);
	}

	NCPP_FORCE_INLINE void A_command_list::ZRS_bind_viewport(const F_viewport& viewport) {

		H_command_list::ZRS_bind_viewport(NCPP_KTHIS(), viewport);
	}

	NCPP_FORCE_INLINE void A_command_list::ZOM_bind_frame_buffer(TKPA_valid<A_frame_buffer> frame_buffer_p) {

		H_command_list::ZOM_bind_frame_buffer(NCPP_KTHIS(), frame_buffer_p);
	}

	NCPP_FORCE_INLINE void A_command_list::ZIA_bind_index_buffer(
		KPA_valid_buffer_handle index_buffer_p,
		u32 offset
	) {
		H_command_list::ZIA_bind_index_buffer(
			NCPP_KTHIS(),
			index_buffer_p,
			offset
		);
	}
	NCPP_FORCE_INLINE void A_command_list::ZIA_bind_input_buffers(
		const TG_span<K_valid_buffer_handle>& input_buffer_p_span,
		const TG_span<u32>& offset_span,
		u32 base_slot_index
	) {
		H_command_list::ZIA_bind_input_buffers(
			NCPP_KTHIS(),
			input_buffer_p_span,
			offset_span,
			base_slot_index
		);
	}
	NCPP_FORCE_INLINE void A_command_list::ZIA_bind_input_buffer(
		KPA_valid_buffer_handle input_buffer_p,
		u32 offset,
		u32 slot_index
	) {
		H_command_list::ZIA_bind_input_buffer(
			NCPP_KTHIS(),
			input_buffer_p,
			offset,
			slot_index
		);
	}

#ifdef NRHI_DRIVER_SUPPORT_SIMPLE_RESOURCE_BINDING
	NCPP_FORCE_INLINE void A_command_list::ZVS_bind_constant_buffers(
		const TG_span<K_valid_buffer_handle>& constant_buffer_p_span,
		u32 base_slot_index
	) {
		H_command_list::ZVS_bind_constant_buffers(
			NCPP_KTHIS(),
			constant_buffer_p_span,
			base_slot_index
		);
	}
	NCPP_FORCE_INLINE void A_command_list::ZVS_bind_constant_buffer(
		KPA_valid_buffer_handle constant_buffer_p,
		u32 slot_index
	) {
		H_command_list::ZVS_bind_constant_buffer(
			NCPP_KTHIS(),
			constant_buffer_p,
			slot_index
		);
	}
	NCPP_FORCE_INLINE void A_command_list::ZVS_bind_srvs(
		const TG_span<K_valid_srv_handle>& srv_p_span,
		u32 base_slot_index
	) {
		H_command_list::ZVS_bind_srvs(
			NCPP_KTHIS(),
			srv_p_span,
			base_slot_index
		);
	}
	NCPP_FORCE_INLINE void A_command_list::ZVS_bind_srv(
		KPA_valid_srv_handle srv_p,
		u32 slot_index
	) {
		H_command_list::ZVS_bind_srv(
			NCPP_KTHIS(),
			srv_p,
			slot_index
		);
	}
	NCPP_FORCE_INLINE void A_command_list::ZVS_bind_sampler_states(
		const TG_span<TK_valid<A_sampler_state>>& sampler_state_p_span,
		u32 base_slot_index
	) {
		H_command_list::ZVS_bind_sampler_states(
			NCPP_KTHIS(),
			sampler_state_p_span,
			base_slot_index
		);
	}
	NCPP_FORCE_INLINE void A_command_list::ZVS_bind_sampler_state(
		TKPA_valid<A_sampler_state> sampler_state_p,
		u32 slot_index
	) {
		H_command_list::ZVS_bind_sampler_state(
			NCPP_KTHIS(),
			sampler_state_p,
			slot_index
		);
	}

	NCPP_FORCE_INLINE void A_command_list::ZPS_bind_constant_buffers(
		const TG_span<K_valid_buffer_handle>& constant_buffer_p_span,
		u32 base_slot_index
	) {
		H_command_list::ZPS_bind_constant_buffers(
			NCPP_KTHIS(),
			constant_buffer_p_span,
			base_slot_index
		);
	}
	NCPP_FORCE_INLINE void A_command_list::ZPS_bind_constant_buffer(
		KPA_valid_buffer_handle constant_buffer_p,
		u32 slot_index
	) {
		H_command_list::ZPS_bind_constant_buffer(
			NCPP_KTHIS(),
			constant_buffer_p,
			slot_index
		);
	}
	NCPP_FORCE_INLINE void A_command_list::ZPS_bind_srvs(
		const TG_span<K_valid_srv_handle>& srv_p_span,
		u32 base_slot_index
	) {
		H_command_list::ZPS_bind_srvs(
			NCPP_KTHIS(),
			srv_p_span,
			base_slot_index
		);
	}
	NCPP_FORCE_INLINE void A_command_list::ZPS_bind_srv(
		KPA_valid_srv_handle srv_p,
		u32 slot_index
	) {
		H_command_list::ZPS_bind_srv(
			NCPP_KTHIS(),
			srv_p,
			slot_index
		);
	}
	NCPP_FORCE_INLINE void A_command_list::ZPS_bind_sampler_states(
		const TG_span<TK_valid<A_sampler_state>>& sampler_state_p_span,
		u32 base_slot_index
	) {
		H_command_list::ZPS_bind_sampler_states(
			NCPP_KTHIS(),
			sampler_state_p_span,
			base_slot_index
		);
	}
	NCPP_FORCE_INLINE void A_command_list::ZPS_bind_sampler_state(
		TKPA_valid<A_sampler_state> sampler_state_p,
		u32 slot_index
	) {
		H_command_list::ZPS_bind_sampler_state(
			NCPP_KTHIS(),
			sampler_state_p,
			slot_index
		);
	}

	NCPP_FORCE_INLINE void A_command_list::ZCS_bind_constant_buffers(
		const TG_span<K_valid_buffer_handle>& constant_buffer_p_span,
		u32 base_slot_index
	) {
		H_command_list::ZCS_bind_constant_buffers(
			NCPP_KTHIS(),
			constant_buffer_p_span,
			base_slot_index
		);
	}
	NCPP_FORCE_INLINE void A_command_list::ZCS_bind_constant_buffer(
		KPA_valid_buffer_handle constant_buffer_p,
		u32 slot_index
	) {
		H_command_list::ZCS_bind_constant_buffer(
			NCPP_KTHIS(),
			constant_buffer_p,
			slot_index
		);
	}
	NCPP_FORCE_INLINE void A_command_list::ZCS_bind_srvs(
		const TG_span<K_valid_srv_handle>& srv_p_span,
		u32 base_slot_index
	) {
		H_command_list::ZCS_bind_srvs(
			NCPP_KTHIS(),
			srv_p_span,
			base_slot_index
		);
	}
	NCPP_FORCE_INLINE void A_command_list::ZCS_bind_srv(
		KPA_valid_srv_handle srv_p,
		u32 slot_index
	) {
		H_command_list::ZCS_bind_srv(
			NCPP_KTHIS(),
			srv_p,
			slot_index
		);
	}
	NCPP_FORCE_INLINE void A_command_list::ZCS_bind_uavs(
		const TG_span<K_valid_uav_handle>& uav_p_span,
		u32 base_slot_index
	) {
		H_command_list::ZCS_bind_uavs(
			NCPP_KTHIS(),
			uav_p_span,
			base_slot_index
		);
	}
	NCPP_FORCE_INLINE void A_command_list::ZCS_bind_uav(
		KPA_valid_uav_handle uav_p,
		u32 slot_index
	) {
		H_command_list::ZCS_bind_uav(
			NCPP_KTHIS(),
			uav_p,
			slot_index
		);
	}
	NCPP_FORCE_INLINE void A_command_list::ZCS_bind_sampler_states(
		const TG_span<TK_valid<A_sampler_state>>& sampler_state_p_span,
		u32 base_slot_index
	) {
		H_command_list::ZCS_bind_sampler_states(
			NCPP_KTHIS(),
			sampler_state_p_span,
			base_slot_index
		);
	}
	NCPP_FORCE_INLINE void A_command_list::ZCS_bind_sampler_state(
		TKPA_valid<A_sampler_state> sampler_state_p,
		u32 slot_index
	) {
		H_command_list::ZCS_bind_sampler_state(
			NCPP_KTHIS(),
			sampler_state_p,
			slot_index
		);
	}
#endif // NRHI_DRIVER_SUPPORT_SIMPLE_RESOURCE_BINDING

#ifdef NRHI_DRIVER_SUPPORT_SIMPLE_WORK_SUBMISSION
	NCPP_FORCE_INLINE void A_command_list::draw(
		u32 vertex_count,
		u32 base_vertex_location
	) {
		NRHI_DRIVER_REQUIRE_SUPPORT_SIMPLE_WORK_SUBMISSION(
			H_command_list::draw(
				NCPP_KTHIS(),
				vertex_count,
				base_vertex_location
			);
		);
	}
	NCPP_FORCE_INLINE void A_command_list::draw_instanced(
		u32 vertex_count_per_instance,
		u32 instance_count,
		u32 base_vertex_location,
		u32 base_instance_location
	) {
		NRHI_DRIVER_REQUIRE_SUPPORT_SIMPLE_WORK_SUBMISSION(
			H_command_list::draw_instanced(
				NCPP_KTHIS(),
				vertex_count_per_instance,
				instance_count,
				base_vertex_location,
				base_instance_location
			);
		);
	}
	NCPP_FORCE_INLINE void A_command_list::draw_indexed(
		u32 index_count,
		u32 base_index_location,
		u32 base_vertex_location
	) {
		NRHI_DRIVER_REQUIRE_SUPPORT_SIMPLE_WORK_SUBMISSION(
			H_command_list::draw_indexed(
				NCPP_KTHIS(),
				index_count,
				base_index_location,
				base_vertex_location
			);
		);
	}
	NCPP_FORCE_INLINE void A_command_list::draw_indexed_instanced(
		u32 index_count_per_instance,
		u32 instance_count,
		u32 base_index_location,
		u32 base_vertex_location,
		u32 base_instance_location
	) {
		NRHI_DRIVER_REQUIRE_SUPPORT_SIMPLE_WORK_SUBMISSION(
			H_command_list::draw_indexed_instanced(
				NCPP_KTHIS(),
				index_count_per_instance,
				instance_count,
				base_index_location,
				base_vertex_location,
				base_instance_location
			);
		);
	}

	NCPP_FORCE_INLINE void A_command_list::draw_instanced_indirect(
		KPA_valid_buffer_handle buffer_p,
		u32 buffer_offset
	) {
		NRHI_DRIVER_REQUIRE_SUPPORT_SIMPLE_WORK_SUBMISSION(
			H_command_list::draw_instanced_indirect(
				NCPP_KTHIS(),
				buffer_p,
				buffer_offset
			);
		);
	}
	NCPP_FORCE_INLINE void A_command_list::draw_indexed_instanced_indirect(
		KPA_valid_buffer_handle buffer_p,
		u32 buffer_offset
	) {
		NRHI_DRIVER_REQUIRE_SUPPORT_SIMPLE_WORK_SUBMISSION(
			H_command_list::draw_indexed_instanced_indirect(
				NCPP_KTHIS(),
				buffer_p,
				buffer_offset
			);
		);
	}

	NCPP_FORCE_INLINE void A_command_list::dispatch_indirect(
		KPA_valid_buffer_handle buffer_p,
		u32 buffer_offset
	) {
		NRHI_DRIVER_REQUIRE_SUPPORT_SIMPLE_WORK_SUBMISSION(
			H_command_list::dispatch_indirect(
				NCPP_KTHIS(),
				buffer_p,
				buffer_offset
			);
		);
	}

	NCPP_FORCE_INLINE void A_command_list::dispatch(PA_vector3_u32 thread_group_count_3d) {

		NRHI_DRIVER_REQUIRE_SUPPORT_SIMPLE_WORK_SUBMISSION(
			H_command_list::dispatch(
				NCPP_KTHIS(),
				thread_group_count_3d
			);
		);
	}
#endif // NRHI_DRIVER_SUPPORT_SIMPLE_WORK_SUBMISSION

#if defined(NRHI_DRIVER_SUPPORT_SIMPLE_RESOURCE_MANAGEMENT) && defined(NRHI_DRIVER_SUPPORT_SIMPLE_RESOURCE_BINDING) && defined(NRHI_DRIVER_SUPPORT_SIMPLE_WORK_SUBMISSION)
	NCPP_FORCE_INLINE void A_command_list::update_resource_data(
		TKPA_valid<A_resource> resource_p,
		void* data_p,
		u32 data_size,
		u32 src_data_offset,
		u32 dst_data_offset
	) {
		NRHI_DRIVER_REQUIRE_SUPPORT_SIMPLE_RESOURCE_MANAGEMENT(
			NRHI_DRIVER_REQUIRE_SUPPORT_SIMPLE_RESOURCE_BINDING(
				NRHI_DRIVER_REQUIRE_SUPPORT_SIMPLE_WORK_SUBMISSION(
					H_command_list::update_resource_data(
						NCPP_KTHIS(),
						resource_p,
						data_p,
						data_size,
						src_data_offset,
						dst_data_offset
					);
				);
			);
		);
	}

	NCPP_FORCE_INLINE void A_command_list::generate_mips(
		KPA_valid_srv_handle srv_p
	) {
		NRHI_DRIVER_REQUIRE_SUPPORT_SIMPLE_RESOURCE_MANAGEMENT(
			NRHI_DRIVER_REQUIRE_SUPPORT_SIMPLE_RESOURCE_BINDING(
				NRHI_DRIVER_REQUIRE_SUPPORT_SIMPLE_WORK_SUBMISSION(
					H_command_list::generate_mips(
						NCPP_KTHIS(),
						srv_p
					);
				);
			);
		);
	}
#endif // NRHI_DRIVER_SUPPORT_SIMPLE_RESOURCE_MANAGEMENT && NRHI_DRIVER_SUPPORT_SIMPLE_RESOURCE_BINDING && NRHI_DRIVER_SUPPORT_SIMPLE_WORK_SUBMISSION

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION
	NCPP_FORCE_INLINE void A_command_list::async_begin(
		TKPA_valid<A_command_allocator> command_allocator_p
	) {
		NRHI_DRIVER_REQUIRE_SUPPORT_ADVANCED_WORK_SUBMISSION(
			H_command_list::async_begin(
				NCPP_KTHIS(),
				command_allocator_p
			);
		);
	}
	NCPP_FORCE_INLINE void A_command_list::async_end() {

		NRHI_DRIVER_REQUIRE_SUPPORT_ADVANCED_WORK_SUBMISSION(
			H_command_list::async_end(
				NCPP_KTHIS()
			);
		);
	}

	NCPP_FORCE_INLINE void A_command_list::async_resource_barrier(
		const F_resource_barrier& resource_barrier
	) {
		NRHI_DRIVER_REQUIRE_SUPPORT_ADVANCED_WORK_SUBMISSION(
			H_command_list::async_resource_barrier(
				NCPP_KTHIS(),
				resource_barrier
			);
		);
	}
	NCPP_FORCE_INLINE void A_command_list::async_resource_barriers(
		const TG_span<F_resource_barrier>& resource_barriers
	) {
		NRHI_DRIVER_REQUIRE_SUPPORT_ADVANCED_WORK_SUBMISSION(
			NRHI_DRIVER_REQUIRE_SUPPORT_ADVANCED_WORK_SUBMISSION(
				H_command_list::async_resource_barriers(
					NCPP_KTHIS(),
					resource_barriers
				);
			);
		);
	}

	NCPP_FORCE_INLINE void A_command_list::async_clear_rtv(
		K_valid_rtv_handle rtv_p,
		PA_vector4_f32 color
	) {
		NRHI_DRIVER_REQUIRE_SUPPORT_ADVANCED_WORK_SUBMISSION(
			H_command_list::async_clear_rtv(
				NCPP_KTHIS(),
				rtv_p,
				color
			);
		);
	}
	NCPP_FORCE_INLINE void A_command_list::async_clear_dsv(
		K_valid_dsv_handle dsv_p,
		ED_clear_flag flag,
		f32 depth,
		u8 stencil
	) {
		NRHI_DRIVER_REQUIRE_SUPPORT_ADVANCED_WORK_SUBMISSION(
			H_command_list::async_clear_dsv(
				NCPP_KTHIS(),
				dsv_p,
				flag,
				depth,
				stencil
			);
		);
	}
#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_BINDING
	NCPP_FORCE_INLINE void A_command_list::async_clear_rtv_with_descriptor(
		F_descriptor_cpu_address rtv_cpu_address,
		PA_vector4_f32 color
	) {
		NRHI_DRIVER_REQUIRE_SUPPORT_ADVANCED_WORK_SUBMISSION(
			H_command_list::async_clear_rtv_with_descriptor(
				NCPP_KTHIS(),
				rtv_cpu_address,
				color
			);
		);
	}
	NCPP_FORCE_INLINE void A_command_list::async_clear_dsv_with_descriptor(
		F_descriptor_cpu_address dsv_cpu_address,
		ED_clear_flag flag,
		f32 depth,
		u8 stencil
	) {
		NRHI_DRIVER_REQUIRE_SUPPORT_ADVANCED_WORK_SUBMISSION(
			NRHI_DRIVER_REQUIRE_SUPPORT_ADVANCED_RESOURCE_BINDING(
				H_command_list::async_clear_dsv_with_descriptor(
					NCPP_KTHIS(),
					dsv_cpu_address,
					flag,
					depth,
					stencil
				);
			);
		);
	}
	NCPP_FORCE_INLINE void A_command_list::async_clear_uav(
        F_descriptor_gpu_address uav_gpu_address,
        F_descriptor_cpu_address uav_cpu_address,
        TKPA_valid<A_resource> resource_p,
        u32 values[4]
    )
    {
		NRHI_DRIVER_REQUIRE_SUPPORT_ADVANCED_WORK_SUBMISSION(
			NRHI_DRIVER_REQUIRE_SUPPORT_ADVANCED_RESOURCE_BINDING(
				H_command_list::async_clear_uav(
					NCPP_KTHIS(),
					uav_gpu_address,
					uav_cpu_address,
					resource_p,
					values
				);
			);
		);
    }
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_BINDING
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_BINDING
	NCPP_FORCE_INLINE void A_command_list::bind_descriptor_heaps(
		const TG_span<TK_valid<A_descriptor_heap>>& descriptor_heap_p_span
	) {
		NRHI_DRIVER_REQUIRE_SUPPORT_ADVANCED_RESOURCE_BINDING(
			H_command_list::bind_descriptor_heaps(
				NCPP_KTHIS(),
				descriptor_heap_p_span
			);
		);
	}

	NCPP_FORCE_INLINE void A_command_list::ZC_bind_root_signature(
		TKPA_valid<A_root_signature> root_signature_p
	) {
		NRHI_DRIVER_REQUIRE_SUPPORT_ADVANCED_RESOURCE_BINDING(
			H_command_list::ZC_bind_root_signature(
				NCPP_KTHIS(),
				root_signature_p
			);
		);
	}
	NCPP_FORCE_INLINE void A_command_list::ZC_bind_root_descriptor_table(
		u32 root_param_index,
		F_descriptor_gpu_address gpu_address
	) {
		NRHI_DRIVER_REQUIRE_SUPPORT_ADVANCED_RESOURCE_BINDING(
			H_command_list::ZC_bind_root_descriptor_table(
				NCPP_KTHIS(),
				root_param_index,
				gpu_address
			);
		);
	}
	NCPP_FORCE_INLINE void A_command_list::ZC_bind_root_constants(
		u32 root_param_index,
		const TG_span<u32>& constant_span,
		u32 offset_in_constants
	) {
		NRHI_DRIVER_REQUIRE_SUPPORT_ADVANCED_RESOURCE_BINDING(
			H_command_list::ZC_bind_root_constants(
				NCPP_KTHIS(),
				root_param_index,
				constant_span,
				offset_in_constants
			);
		);
	}
	NCPP_FORCE_INLINE void A_command_list::ZC_bind_root_constant(
		u32 root_param_index,
		u32 root_constant,
		u32 offset_in_constants
	) {
		NRHI_DRIVER_REQUIRE_SUPPORT_ADVANCED_RESOURCE_BINDING(
			H_command_list::ZC_bind_root_constant(
				NCPP_KTHIS(),
				root_param_index,
				root_constant,
				offset_in_constants
			);
		);
	}
	NCPP_FORCE_INLINE void A_command_list::ZC_bind_root_srv(
		u32 root_param_index,
		KPA_valid_srv_handle srv_p
	) {
		NRHI_DRIVER_REQUIRE_SUPPORT_ADVANCED_RESOURCE_BINDING(
			H_command_list::ZC_bind_root_srv(
				NCPP_KTHIS(),
				root_param_index,
				srv_p
			);
		);
	}
	NCPP_FORCE_INLINE void A_command_list::ZC_bind_root_uav(
		u32 root_param_index,
		KPA_valid_uav_handle uav_p
	) {
		NRHI_DRIVER_REQUIRE_SUPPORT_ADVANCED_RESOURCE_BINDING(
			H_command_list::ZC_bind_root_uav(
				NCPP_KTHIS(),
				root_param_index,
				uav_p
			);
		);
	}
	NCPP_FORCE_INLINE void A_command_list::ZC_bind_root_cbv_with_resource(
		u32 root_param_index,
		TKPA_valid<A_resource> resource_p
	) {
		NRHI_DRIVER_REQUIRE_SUPPORT_ADVANCED_RESOURCE_BINDING(
			H_command_list::ZC_bind_root_cbv_with_resource(
				NCPP_KTHIS(),
				root_param_index,
				resource_p
			);
		);
	}
	NCPP_FORCE_INLINE void A_command_list::ZC_bind_root_srv_with_resource(
		u32 root_param_index,
		TKPA_valid<A_resource> resource_p
	) {
		NRHI_DRIVER_REQUIRE_SUPPORT_ADVANCED_RESOURCE_BINDING(
			H_command_list::ZC_bind_root_srv_with_resource(
				NCPP_KTHIS(),
				root_param_index,
				resource_p
			);
		);
	}
	NCPP_FORCE_INLINE void A_command_list::ZC_bind_root_uav_with_resource(
		u32 root_param_index,
		TKPA_valid<A_resource> resource_p
	) {
		NRHI_DRIVER_REQUIRE_SUPPORT_ADVANCED_RESOURCE_BINDING(
			H_command_list::ZC_bind_root_uav_with_resource(
				NCPP_KTHIS(),
				root_param_index,
				resource_p
			);
		);
	}
	NCPP_FORCE_INLINE void A_command_list::ZC_bind_root_cbv_with_gpu_virtual_address(
		u32 root_param_index,
		F_resource_gpu_virtual_address gpu_virtual_address
	) {
		NRHI_DRIVER_REQUIRE_SUPPORT_ADVANCED_RESOURCE_BINDING(
			H_command_list::ZC_bind_root_cbv_with_gpu_virtual_address(
				NCPP_KTHIS(),
				root_param_index,
				gpu_virtual_address
			);
		);
	}
	NCPP_FORCE_INLINE void A_command_list::ZC_bind_root_srv_with_gpu_virtual_address(
		u32 root_param_index,
		F_resource_gpu_virtual_address gpu_virtual_address
	) {
		NRHI_DRIVER_REQUIRE_SUPPORT_ADVANCED_RESOURCE_BINDING(
			H_command_list::ZC_bind_root_srv_with_gpu_virtual_address(
				NCPP_KTHIS(),
				root_param_index,
				gpu_virtual_address
			);
		);
	}
	NCPP_FORCE_INLINE void A_command_list::ZC_bind_root_uav_with_gpu_virtual_address(
		u32 root_param_index,
		F_resource_gpu_virtual_address gpu_virtual_address
	) {
		NRHI_DRIVER_REQUIRE_SUPPORT_ADVANCED_RESOURCE_BINDING(
			H_command_list::ZC_bind_root_uav_with_gpu_virtual_address(
				NCPP_KTHIS(),
				root_param_index,
				gpu_virtual_address
			);
		);
	}

	NCPP_FORCE_INLINE void A_command_list::ZG_bind_root_signature(
		TKPA_valid<A_root_signature> root_signature_p
	) {
		NRHI_DRIVER_REQUIRE_SUPPORT_ADVANCED_RESOURCE_BINDING(
			H_command_list::ZG_bind_root_signature(
				NCPP_KTHIS(),
				root_signature_p
			);
		);
	}
	NCPP_FORCE_INLINE void A_command_list::ZG_bind_root_descriptor_table(
		u32 root_param_index,
		F_descriptor_gpu_address gpu_address
	) {
		NRHI_DRIVER_REQUIRE_SUPPORT_ADVANCED_RESOURCE_BINDING(
			H_command_list::ZG_bind_root_descriptor_table(
				NCPP_KTHIS(),
				root_param_index,
				gpu_address
			);
		);
	}
	NCPP_FORCE_INLINE void A_command_list::ZG_bind_root_constants(
		u32 root_param_index,
		const TG_span<u32>& constant_span,
		u32 offset_in_constants
	) {
		NRHI_DRIVER_REQUIRE_SUPPORT_ADVANCED_RESOURCE_BINDING(
			H_command_list::ZG_bind_root_constants(
				NCPP_KTHIS(),
				root_param_index,
				constant_span,
				offset_in_constants
			);
		);
	}
	NCPP_FORCE_INLINE void A_command_list::ZG_bind_root_constant(
		u32 root_param_index,
		u32 root_constant,
		u32 offset_in_constants
	) {
		NRHI_DRIVER_REQUIRE_SUPPORT_ADVANCED_RESOURCE_BINDING(
			H_command_list::ZG_bind_root_constant(
				NCPP_KTHIS(),
				root_param_index,
				root_constant,
				offset_in_constants
			);
		);
	}
	NCPP_FORCE_INLINE void A_command_list::ZG_bind_root_srv(
		u32 root_param_index,
		KPA_valid_srv_handle srv_p
	) {
		NRHI_DRIVER_REQUIRE_SUPPORT_ADVANCED_RESOURCE_BINDING(
			H_command_list::ZG_bind_root_srv(
				NCPP_KTHIS(),
				root_param_index,
				srv_p
			);
		);
	}
	NCPP_FORCE_INLINE void A_command_list::ZG_bind_root_uav(
		u32 root_param_index,
		KPA_valid_uav_handle uav_p
	) {
		NRHI_DRIVER_REQUIRE_SUPPORT_ADVANCED_RESOURCE_BINDING(
			H_command_list::ZG_bind_root_uav(
				NCPP_KTHIS(),
				root_param_index,
				uav_p
			);
		);
	}
	NCPP_FORCE_INLINE void A_command_list::ZG_bind_root_cbv_with_resource(
		u32 root_param_index,
		TKPA_valid<A_resource> resource_p
	) {
		NRHI_DRIVER_REQUIRE_SUPPORT_ADVANCED_RESOURCE_BINDING(
			H_command_list::ZG_bind_root_cbv_with_resource(
				NCPP_KTHIS(),
				root_param_index,
				resource_p
			);
		);
	}
	NCPP_FORCE_INLINE void A_command_list::ZG_bind_root_srv_with_resource(
		u32 root_param_index,
		TKPA_valid<A_resource> resource_p
	) {
		NRHI_DRIVER_REQUIRE_SUPPORT_ADVANCED_RESOURCE_BINDING(
			H_command_list::ZG_bind_root_srv_with_resource(
				NCPP_KTHIS(),
				root_param_index,
				resource_p
			);
		);
	}
	NCPP_FORCE_INLINE void A_command_list::ZG_bind_root_uav_with_resource(
		u32 root_param_index,
		TKPA_valid<A_resource> resource_p
	) {
		NRHI_DRIVER_REQUIRE_SUPPORT_ADVANCED_RESOURCE_BINDING(
			H_command_list::ZG_bind_root_uav_with_resource(
				NCPP_KTHIS(),
				root_param_index,
				resource_p
			);
		);
	}
	NCPP_FORCE_INLINE void A_command_list::ZG_bind_root_cbv_with_gpu_virtual_address(
		u32 root_param_index,
		F_resource_gpu_virtual_address gpu_virtual_address
	) {
		NRHI_DRIVER_REQUIRE_SUPPORT_ADVANCED_RESOURCE_BINDING(
			H_command_list::ZG_bind_root_cbv_with_gpu_virtual_address(
				NCPP_KTHIS(),
				root_param_index,
				gpu_virtual_address
			);
		);
	}
	NCPP_FORCE_INLINE void A_command_list::ZG_bind_root_srv_with_gpu_virtual_address(
		u32 root_param_index,
		F_resource_gpu_virtual_address gpu_virtual_address
	) {
		NRHI_DRIVER_REQUIRE_SUPPORT_ADVANCED_RESOURCE_BINDING(
			H_command_list::ZG_bind_root_srv_with_gpu_virtual_address(
				NCPP_KTHIS(),
				root_param_index,
				gpu_virtual_address
			);
		);
	}
	NCPP_FORCE_INLINE void A_command_list::ZG_bind_root_uav_with_gpu_virtual_address(
		u32 root_param_index,
		F_resource_gpu_virtual_address gpu_virtual_address
	) {
		NRHI_DRIVER_REQUIRE_SUPPORT_ADVANCED_RESOURCE_BINDING(
			H_command_list::ZG_bind_root_uav_with_gpu_virtual_address(
				NCPP_KTHIS(),
				root_param_index,
				gpu_virtual_address
			);
		);
	}
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_BINDING

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
	NCPP_FORCE_INLINE void A_command_list::ZIA_bind_index_buffer_with_gpu_virtual_address(
		F_resource_gpu_virtual_address gpu_virtual_address,
		u32 size,
		ED_format format
	) {
		NRHI_DRIVER_REQUIRE_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT(
			H_command_list::ZIA_bind_index_buffer_with_gpu_virtual_address(
				NCPP_KTHIS(),
				gpu_virtual_address,
				size,
				format
			);
		);
	}
	NCPP_FORCE_INLINE void A_command_list::ZIA_bind_input_buffers_with_gpu_virtual_address(
		TG_span<F_resource_gpu_virtual_address> gpu_virtual_addresses,
		TG_span<u32> sizes,
		TG_span<u32> strides,
		u32 base_slot_index
	) {
		NRHI_DRIVER_REQUIRE_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT(
			H_command_list::ZIA_bind_input_buffers_with_gpu_virtual_address(
				NCPP_KTHIS(),
				gpu_virtual_addresses,
				sizes,
				strides,
				base_slot_index
			);
		);
	}
	NCPP_FORCE_INLINE void A_command_list::ZIA_bind_input_buffer_with_gpu_virtual_address(
		F_resource_gpu_virtual_address gpu_virtual_address,
		u32 size,
		u32 stride,
		u32 slot_index
	) {
		NRHI_DRIVER_REQUIRE_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT(
			H_command_list::ZIA_bind_input_buffer_with_gpu_virtual_address(
				NCPP_KTHIS(),
				gpu_virtual_address,
				size,
				stride,
				slot_index
			);
		);
	}
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION
	NCPP_FORCE_INLINE void A_command_list::async_copy_resource(
		TKPA_valid<A_resource> dst_resource_p,
		TKPA_valid<A_resource> src_resource_p
	) {
		NRHI_DRIVER_REQUIRE_SUPPORT_ADVANCED_WORK_SUBMISSION(
			H_command_list::async_copy_resource(
				NCPP_KTHIS(),
				dst_resource_p,
				src_resource_p
			);
		);
	}
	NCPP_FORCE_INLINE void A_command_list::async_copy_buffer_region(
		TKPA_valid<A_resource> dst_resource_p,
		TKPA_valid<A_resource> src_resource_p,
		u64 dst_offset,
		u64 src_offset,
		u64 size
	) {
		NRHI_DRIVER_REQUIRE_SUPPORT_ADVANCED_WORK_SUBMISSION(
			H_command_list::async_copy_buffer_region(
				NCPP_KTHIS(),
				dst_resource_p,
				src_resource_p,
				dst_offset,
				src_offset,
				size
			);
		);
	}
	NCPP_FORCE_INLINE void A_command_list::async_copy_texture_region(
		const F_texture_copy_location& dst_location,
		const F_texture_copy_location& src_location,
		PA_vector3_u32 dst_coord,
		PA_vector3_u32 src_coord,
		PA_vector3_u32 volume
	)
	{
		NRHI_DRIVER_REQUIRE_SUPPORT_ADVANCED_WORK_SUBMISSION(
			H_command_list::async_copy_texture_region(
				NCPP_KTHIS(),
				dst_location,
				src_location,
				dst_coord,
				src_coord,
				volume
			);
		);
	}

	NCPP_FORCE_INLINE void A_command_list::async_draw(
		u32 vertex_count,
		u32 base_vertex_location
	) {
		NRHI_DRIVER_REQUIRE_SUPPORT_ADVANCED_WORK_SUBMISSION(
			H_command_list::async_draw(
				NCPP_KTHIS(),
				vertex_count,
				base_vertex_location
			);
		);
	}
	NCPP_FORCE_INLINE void A_command_list::async_draw_instanced(
		u32 vertex_count_per_instance,
		u32 instance_count,
		u32 base_vertex_location,
		u32 base_instance_location
	) {
		NRHI_DRIVER_REQUIRE_SUPPORT_ADVANCED_WORK_SUBMISSION(
			H_command_list::async_draw_instanced(
				NCPP_KTHIS(),
				vertex_count_per_instance,
				instance_count,
				base_vertex_location,
				base_instance_location
			);
		);
	}
	NCPP_FORCE_INLINE void A_command_list::async_draw_indexed(
		u32 index_count,
		u32 base_index_location,
		u32 base_vertex_location
	) {
		NRHI_DRIVER_REQUIRE_SUPPORT_ADVANCED_WORK_SUBMISSION(
			H_command_list::async_draw_indexed(
				NCPP_KTHIS(),
				index_count,
				base_index_location,
				base_vertex_location
			);
		);
	}
	NCPP_FORCE_INLINE void A_command_list::async_draw_indexed_instanced(
		u32 index_count_per_instance,
		u32 instance_count,
		u32 base_index_location,
		u32 base_vertex_location,
		u32 base_instance_location
	) {
		NRHI_DRIVER_REQUIRE_SUPPORT_ADVANCED_WORK_SUBMISSION(
			H_command_list::async_draw_indexed_instanced(
				NCPP_KTHIS(),
				index_count_per_instance,
				instance_count,
				base_index_location,
				base_vertex_location,
				base_instance_location
			);
		);
	}

	NCPP_FORCE_INLINE void A_command_list::async_dispatch(
		PA_vector3_u32 thread_group_count_3d
	) {
		NRHI_DRIVER_REQUIRE_SUPPORT_ADVANCED_WORK_SUBMISSION(
			H_command_list::async_dispatch(
				NCPP_KTHIS(),
				thread_group_count_3d
			);
		);
	}

	NCPP_FORCE_INLINE void A_command_list::async_dispatch_mesh(
		PA_vector3_u32 thread_group_count_3d
	) {
		NRHI_DRIVER_REQUIRE_SUPPORT_ADVANCED_WORK_SUBMISSION(
			H_command_list::async_dispatch_mesh(
				NCPP_KTHIS(),
				thread_group_count_3d
			);
		);
	}
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_INDIRECT_COMMANDS
	NCPP_FORCE_INLINE void A_command_list::async_execute_indirect(
		TKPA_valid<A_command_signature> command_signature_p,
		u32 command_count,
		KPA_valid_buffer_handle argument_buffer_p,
		u64 argument_buffer_offset_in_bytes
	) {
		NRHI_DRIVER_REQUIRE_SUPPORT_ADVANCED_INDIRECT_COMMANDS(
			H_command_list::async_execute_indirect(
				NCPP_KTHIS(),
				command_signature_p,
				command_count,
				argument_buffer_p,
				argument_buffer_offset_in_bytes
			);
		);
	}
	NCPP_FORCE_INLINE void A_command_list::async_execute_indirect_with_dynamic_count(
		TKPA_valid<A_command_signature> command_signature_p,
		u32 max_command_count,
		KPA_valid_buffer_handle argument_buffer_p,
		u64 argument_buffer_offset_in_bytes,
		KPA_valid_buffer_handle count_buffer_p,
		u64 count_buffer_offset_in_bytes
	) {
		NRHI_DRIVER_REQUIRE_SUPPORT_ADVANCED_INDIRECT_COMMANDS(
			H_command_list::async_execute_indirect_with_dynamic_count(
				NCPP_KTHIS(),
				command_signature_p,
				max_command_count,
				argument_buffer_p,
				argument_buffer_offset_in_bytes,
				count_buffer_p,
				count_buffer_offset_in_bytes
			);
		);
	}
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_INDIRECT_COMMANDS

#ifdef NRHI_DRIVER_SUPPORT_STATE_OBJECT
	NCPP_FORCE_INLINE void A_command_list::async_bind_generic_program(
		const F_program_id& program_id
	)
	{
		NCPP_ASSERT(H_driver::support_state_object());
		H_command_list::async_bind_generic_program(
			NCPP_KTHIS(),
			program_id
		);
	}
#ifdef NRHI_DRIVER_SUPPORT_WORK_GRAPHS
	NCPP_FORCE_INLINE void A_command_list::async_bind_work_graph_program(
		const F_program_id& program_id,
		ED_bind_work_graph_program_flag flags,
		F_resource_gpu_virtual_address backing_memory_gpu_address
	)
	{
		NCPP_ASSERT(H_driver::support_work_graphs());
		H_command_list::async_bind_work_graph_program(
			NCPP_KTHIS(),
			program_id,
			flags,
			backing_memory_gpu_address
		);
	}
#endif
#endif

#ifdef NRHI_DRIVER_SUPPORT_WORK_GRAPHS
	NCPP_FORCE_INLINE void A_command_list::async_dispatch_graph(const F_dispatch_graph_desc& desc)
	{
		NCPP_ASSERT(H_driver::support_work_graphs());
		H_command_list::async_dispatch_graph(
			NCPP_KTHIS(),
			desc
		);
	}
#endif
}
