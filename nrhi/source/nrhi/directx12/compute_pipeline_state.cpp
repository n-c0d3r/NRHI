#include <nrhi/directx12/compute_pipeline_state.hpp>
#include <nrhi/directx12/shader.hpp>
#include <nrhi/directx12/device.hpp>
#include <nrhi/directx12/root_signature.hpp>
#include <nrhi/shader_type.hpp>



namespace nrhi {

	F_directx12_compute_pipeline_state::F_directx12_compute_pipeline_state(
		TKPA_valid<A_device> device_p,
		const F_pipeline_state_desc& desc,
		F_directx12_pipeline_state_direct_flag,
		ED_pipeline_state_type overrided_type
	) :
		F_directx12_pipeline_state(
			device_p,
			desc,
			overrided_type,
			create_d3d12_compute_pipeline_state_direct(
				device_p,
				desc
			)
		)
	{
	}
	F_directx12_compute_pipeline_state::F_directx12_compute_pipeline_state(
		TKPA_valid<A_device> device_p,
		const F_pipeline_state_desc& desc,
		ED_pipeline_state_type overrided_type,
		ID3D12PipelineState* d3d12_compute_pipeline_state_p
	) :
		F_directx12_pipeline_state(device_p, desc, overrided_type, d3d12_compute_pipeline_state_p)
	{
	}
	F_directx12_compute_pipeline_state::~F_directx12_compute_pipeline_state(){
	}

	ID3D12PipelineState* F_directx12_compute_pipeline_state::create_d3d12_compute_pipeline_state_direct(
		TKPA_valid<A_device> device_p,
		const F_pipeline_state_desc& desc
	) {
		return 0;
	}

}