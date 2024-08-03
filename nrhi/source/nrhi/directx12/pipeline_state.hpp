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



	class NRHI_API F_directx12_pipeline_state : public A_pipeline_state {

	protected:
		ID3D12PipelineState* d3d12_pipeline_state_p_ = 0;

	public:
		NCPP_FORCE_INLINE ID3D12PipelineState* d3d12_pipeline_state_p() noexcept { return d3d12_pipeline_state_p_; }
		NCPP_FORCE_INLINE void set_d3d12_pipeline_state_p_unsafe(ID3D12PipelineState* value) noexcept { d3d12_pipeline_state_p_ = value; }



	public:
		F_directx12_pipeline_state(
			TKPA_valid<A_device> device_p,
			ED_pipeline_state_type overrided_type,
			TKPA_valid<A_root_signature> root_signature_p,
			ID3D12PipelineState* d3d12_pipeline_state_p
		);
		virtual ~F_directx12_pipeline_state();

	};



	class NRHI_API HD_directx12_pipeline_state {

	public:
		static TU<A_pipeline_state> create_with_root_signature(
			TKPA_valid<A_device> device_p,
			const A_pipeline_state_desc& desc,
			TKPA_valid<A_root_signature> root_signature_p
		);

	public:
		static U_graphics_pipeline_state_handle create_graphics_pipeline_state_with_root_signature(
			TKPA_valid<A_device> device_p,
			const F_graphics_pipeline_state_options& options,
			TKPA_valid<A_root_signature> root_signature_p
		);
		static U_compute_pipeline_state_handle create_compute_pipeline_state_with_root_signature(
			TKPA_valid<A_device> device_p,
			const F_compute_pipeline_state_options& options,
			TKPA_valid<A_root_signature> root_signature_p
		);

	public:
		static const F_graphics_pipeline_state_options& graphics_options(
			KPA_valid_graphics_pipeline_state_handle pso_p
		);
		static const F_compute_pipeline_state_options& compute_options(
			KPA_valid_compute_pipeline_state_handle pso_p
		);



#pragma region Alternative Functions
#ifdef NRHI_DRIVER_ENABLE_INTERFACE_ONLY_SUPPORTS
	public:
		static TU<A_pipeline_state> create(
			TKPA_valid<A_device> device_p,
			const A_pipeline_state_desc& desc
		);

	public:
		static U_graphics_pipeline_state_handle create_graphics_pipeline_state(
			TKPA_valid<A_device> device_p,
			const F_graphics_pipeline_state_options& options
		);
		static U_compute_pipeline_state_handle create_compute_pipeline_state(
			TKPA_valid<A_device> device_p,
			const F_compute_pipeline_state_options& options
		);
#endif // NRHI_DRIVER_ENABLE_INTERFACE_ONLY_SUPPORTS
#pragma endregion

	};

}
