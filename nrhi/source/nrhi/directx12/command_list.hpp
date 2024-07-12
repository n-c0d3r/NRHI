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
#include <nrhi/shader_blob_desc.hpp>
#include <nrhi/resource_view_handle.hpp>

#pragma endregion



namespace nrhi {

    class A_device;



    class NRHI_API F_directx12_command_list : public A_command_list {

	public:
		friend class HD_directx12_command_list;



	private:
		ID3D12GraphicsCommandList* d3d12_command_list_p_ = 0;

	public:
		NCPP_FORCE_INLINE ID3D12GraphicsCommandList* d3d12_command_list_p() const noexcept { return d3d12_command_list_p_; }



    public:
        F_directx12_command_list(TKPA_valid<A_device> device_p, const F_command_list_desc& desc);
        ~F_directx12_command_list();

    };



    class NRHI_API HD_directx12_command_list {

    public:
        static TU<A_command_list> create(TKPA_valid<A_device> device_p, const F_command_list_desc& desc);

    };

}
