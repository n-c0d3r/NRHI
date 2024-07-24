#pragma once

/** @file nrhi/directx12/pipeline_state.hpp
*
*   Implement directx12 pipeline_state.
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

#include <nrhi/pipeline_state_base.hpp>
#include <nrhi/graphics_pipeline_state.hpp>
#include <nrhi/compute_pipeline_state.hpp>

#pragma endregion



namespace nrhi {

	class A_device;



	struct F_directx12_pipeline_state_direct_flag {};



	class NRHI_API F_directx12_pipeline_state : public A_pipeline_state {

	protected:
		ID3D12PipelineState* d3d12_pipeline_state_p_ = 0;

	public:
		NCPP_FORCE_INLINE ID3D12PipelineState* d3d12_pipeline_state_p() noexcept { return d3d12_pipeline_state_p_; }
		NCPP_FORCE_INLINE void set_d3d12_pipeline_state_p_unsafe(ID3D12PipelineState* value) noexcept { d3d12_pipeline_state_p_ = value; }



	public:
		F_directx12_pipeline_state(
			TKPA_valid<A_device> device_p,
			const F_pipeline_state_desc& desc,
			ED_pipeline_state_type overrided_type,
			ID3D12PipelineState* d3d12_pipeline_state_p
		);
		virtual ~F_directx12_pipeline_state();

	};



	class NRHI_API HD_directx12_pipeline_state {

	public:
		static TU<A_pipeline_state> create_direct(
			TKPA_valid<A_device> device_p,
			const F_pipeline_state_desc& desc
		);

	public:
		static U_graphics_pipeline_state_handle create_graphics_pipeline_state_direct(
			TKPA_valid<A_device> device_p,
			const F_pipeline_state_desc& desc
		);
		static U_compute_pipeline_state_handle create_compute_pipeline_state_direct(
			TKPA_valid<A_device> device_p,
			const F_pipeline_state_desc& desc
		);

	};

}
