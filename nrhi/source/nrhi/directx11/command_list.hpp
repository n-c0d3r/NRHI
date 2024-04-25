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

#pragma endregion



namespace nrhi {

    class A_device;
    class A_pipeline_state;
    class A_frame_buffer;



    class NRHI_API F_directx11_command_list : public A_command_list {

    private:
        ID3D11DeviceContext* d3d11_device_context_p_ = 0;

    public:
        NCPP_FORCE_INLINE ID3D11DeviceContext* d3d11_device_context_p() noexcept { return d3d11_device_context_p_; }



    public:
        F_directx11_command_list(TKPA_valid<A_device> device_p, const F_command_list_desc& desc);
        ~F_directx11_command_list();

    };



    class NRHI_API HD_directx11_command_list {

    public:
        static TU<A_command_list> create(TKPA_valid<A_device> device_p, const F_command_list_desc& desc);

	public:
		static void set_frame_buffer(
			TKPA_valid<A_command_list> command_list_p,
			TKPA_valid<A_frame_buffer> frame_buffer_p
		);
		static void set_graphics_pipeline_state(
			TKPA_valid<A_command_list> command_list_p,
			KPA_valid_graphics_pipeline_state_handle graphics_pipeline_state_p
		);
		static void set_compute_pipeline_state(
			TKPA_valid<A_command_list> command_list_p,
			KPA_valid_compute_pipeline_state_handle compute_pipeline_state_p
		);
		static void draw_indexed(
			TKPA_valid<A_command_list> command_list_p,
			u32 index_count,
			u32 base_index_location
		);

    };

}
