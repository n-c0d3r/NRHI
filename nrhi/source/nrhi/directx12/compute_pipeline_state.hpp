#pragma once

/** @file nrhi/directx12/compute_pipeline_state.hpp
*
*   Implement directx12 compute pipeline_state.
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

#include <nrhi/directx12/pipeline_state.hpp>

#pragma endregion



namespace nrhi {

	class A_device;



	class NRHI_API F_directx12_compute_pipeline_state : public F_directx12_pipeline_state {

	private:
		F_compute_pipeline_state_options options_;

	public:
		NCPP_FORCE_INLINE const auto& options() const noexcept { return options_; }



	public:
		F_directx12_compute_pipeline_state(
			TKPA_valid<A_device> device_p,
			const F_compute_pipeline_state_options& options,
			TKPA_valid<A_root_signature> root_signature_p
		);
		F_directx12_compute_pipeline_state(
			TKPA_valid<A_device> device_p,
			const F_compute_pipeline_state_options& options,
			TKPA_valid<A_root_signature> root_signature_p,
			ID3D12PipelineState* d3d12_pipeline_state_p
		);
		virtual ~F_directx12_compute_pipeline_state();

	private:
		static ID3D12PipelineState* create_d3d12_compute_pipeline_state(
			TKPA_valid<A_device> device_p,
			const F_compute_pipeline_state_options& options,
			TKPA_valid<A_root_signature> root_signature_p
		);

	};

}
