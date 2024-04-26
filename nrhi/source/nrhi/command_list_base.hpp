#pragma once

/** @file nrhi/command_list_base.hpp
*
*   Implement command list base class.
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

#include <nrhi/command_list_type.hpp>
#include <nrhi/graphics_pipeline_state_handle.hpp>
#include <nrhi/compute_pipeline_state_handle.hpp>
#include <nrhi/resource_view_handle.hpp>
#include <nrhi/buffer_handle.hpp>

#pragma endregion



namespace nrhi {

    class A_device;
    class A_frame_buffer;



    struct F_command_list_desc {

        E_command_list_type type;

    };



    class NRHI_API A_command_list {

    private:
        TK_valid<A_device> device_p_;
        F_command_list_desc desc_;

    public:
        NCPP_FORCE_INLINE TK_valid<A_device> device_p() noexcept { return device_p_; }
        NCPP_FORCE_INLINE const F_command_list_desc& desc() const noexcept { return desc_; }



    protected:
        A_command_list(TKPA_valid<A_device> device_p, const F_command_list_desc& desc);

    public:
        virtual ~A_command_list();



	public:
		void clear_state();
		void set_frame_buffer(TKPA_valid<A_frame_buffer> frame_buffer_p);
		void clear_rtv(KPA_valid_rtv_handle rtv_p, PA_vector4 color);
		void set_graphics_pipeline_state(KPA_valid_graphics_pipeline_state_handle graphics_pipeline_state_p);
		void set_compute_pipeline_state(KPA_valid_compute_pipeline_state_handle compute_pipeline_state_p);
		void set_vertex_buffers(
			const TG_span<K_valid_buffer_handle>& vertex_buffer_p_span,
			const TG_span<u32>& offset_span
		);
		void set_instance_buffers(
			const TG_span<K_valid_buffer_handle>& instance_buffer_p_span,
			const TG_span<u32>& offset_span
		);
		void set_index_buffer(
			KPA_valid_buffer_handle index_buffer_p,
			u32 offset
		);
		void draw_indexed(u32 index_count, u32 base_index_location);

    };

}
