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

	NCPP_FORCE_INLINE void A_command_list::clear_state() {

		H_command_list::clear_state(NCPP_KTHIS());
	}
	NCPP_FORCE_INLINE void A_command_list::clear_rtv(KPA_valid_rtv_handle rtv_p, PA_vector4 color) {

		H_command_list::clear_rtv(NCPP_KTHIS(), rtv_p, color);
	}
	NCPP_FORCE_INLINE void A_command_list::clear_dsv(
		KPA_valid_dsv_handle dsv_p,
		E_clear_flag flag,
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

	NCPP_FORCE_INLINE void A_command_list::bind_graphics_pipeline_state(KPA_valid_graphics_pipeline_state_handle graphics_pipeline_state_p) {

		H_command_list::bind_graphics_pipeline_state(NCPP_KTHIS(), graphics_pipeline_state_p);
	}
	NCPP_FORCE_INLINE void A_command_list::bind_compute_pipeline_state(KPA_valid_compute_pipeline_state_handle compute_pipeline_state_p) {

		H_command_list::bind_compute_pipeline_state(NCPP_KTHIS(), compute_pipeline_state_p);
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
	NCPP_FORCE_INLINE void A_command_list::ZIA_bind_vertex_buffers(
		const TG_span<K_valid_buffer_handle>& vertex_buffer_p_span,
		const TG_span<u32>& offset_span,
		u32 base_slot_index
	) {

		H_command_list::ZIA_bind_vertex_buffers(
			NCPP_KTHIS(),
			vertex_buffer_p_span,
			offset_span,
			base_slot_index
		);
	}
	NCPP_FORCE_INLINE void A_command_list::ZIA_bind_vertex_buffer(
		KPA_valid_buffer_handle vertex_buffer_p,
		u32 offset,
		u32 slot_index
	) {

		H_command_list::ZIA_bind_vertex_buffer(
			NCPP_KTHIS(),
			vertex_buffer_p,
			offset,
			slot_index
		);
	}
	NCPP_FORCE_INLINE void A_command_list::ZIA_bind_instance_buffers(
		const TG_span<K_valid_buffer_handle>& instance_buffer_p_span,
		const TG_span<u32>& offset_span,
		u32 base_slot_index
	) {

		H_command_list::ZIA_bind_instance_buffers(
			NCPP_KTHIS(),
			instance_buffer_p_span,
			offset_span,
			base_slot_index
		);
	}
	NCPP_FORCE_INLINE void A_command_list::ZIA_bind_instance_buffer(
		KPA_valid_buffer_handle instance_buffer_p,
		u32 offset,
		u32 slot_index
	) {

		H_command_list::ZIA_bind_instance_buffer(
			NCPP_KTHIS(),
			instance_buffer_p,
			offset,
			slot_index
		);
	}

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

	NCPP_FORCE_INLINE void A_command_list::ZOM_bind_frame_buffer(TKPA_valid<A_frame_buffer> frame_buffer_p) {

		H_command_list::ZOM_bind_frame_buffer(NCPP_KTHIS(), frame_buffer_p);
	}

	NCPP_FORCE_INLINE void A_command_list::draw_indexed(
		u32 index_count,
		u32 base_index_location,
		u32 base_vertex_location
	) {

		H_command_list::draw_indexed(
			NCPP_KTHIS(),
			index_count,
			base_index_location,
			base_vertex_location
		);
	}
	NCPP_FORCE_INLINE void A_command_list::draw_indexed_instanced(
		u32 index_count_per_instance,
		u32 instance_count,
		u32 base_index_location,
		u32 base_vertex_location,
		u32 base_instance_location
	) {

		H_command_list::draw_indexed_instanced(
			NCPP_KTHIS(),
			index_count_per_instance,
			instance_count,
			base_index_location,
			base_vertex_location,
			base_instance_location
		);
	}

}
