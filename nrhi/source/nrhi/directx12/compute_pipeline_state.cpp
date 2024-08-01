#include <nrhi/directx12/compute_pipeline_state.hpp>
#include <nrhi/directx12/device.hpp>
#include <nrhi/directx12/root_signature.hpp>
#include <nrhi/shader_type.hpp>



namespace nrhi {

	F_directx12_compute_pipeline_state::F_directx12_compute_pipeline_state(
		TKPA_valid<A_device> device_p,
		const F_compute_pipeline_state_options& options,
		TKPA_valid<A_root_signature> root_signature_p
	) :
		F_directx12_pipeline_state(
			device_p,
			ED_pipeline_state_type::COMPUTE,
			root_signature_p,
			create_d3d12_compute_pipeline_state(
				device_p,
				options,
				root_signature_p
			)
		),
		options_(options)
	{
	}
	F_directx12_compute_pipeline_state::F_directx12_compute_pipeline_state(
		TKPA_valid<A_device> device_p,
		const F_compute_pipeline_state_options& options,
		TKPA_valid<A_root_signature> root_signature_p,
		ID3D12PipelineState* d3d12_graphics_pipeline_state_p
	) :
		F_directx12_pipeline_state(
			device_p,
			ED_pipeline_state_type::GRAPHICS,
			root_signature_p,
			d3d12_graphics_pipeline_state_p
		),
		options_(options)
	{
	}
	F_directx12_compute_pipeline_state::~F_directx12_compute_pipeline_state(){

		if(d3d12_pipeline_state_p_)
			d3d12_pipeline_state_p_->Release();
	}

	ID3D12PipelineState* F_directx12_compute_pipeline_state::create_d3d12_compute_pipeline_state(
		TKPA_valid<A_device> device_p,
		const F_compute_pipeline_state_options& options,
		TKPA_valid<A_root_signature> root_signature_p
	) {
		ID3D12Device* d3d12_device_p = device_p.T_cast<F_directx12_device>()->d3d12_device_p();
		ID3D12RootSignature* d3d12_root_signature_p = root_signature_p.T_cast<F_directx12_root_signature>()->d3d12_root_signature_p();

		ID3D12PipelineState* d3d12_pipeline_state_p = 0;

		D3D12_COMPUTE_PIPELINE_STATE_DESC  d3d12_pipeline_state_desc = {};
		d3d12_pipeline_state_desc.pRootSignature = d3d12_root_signature_p;

		const auto& shader_binary = options.shader_binaries.compute;
		d3d12_pipeline_state_desc.CS = { shader_binary.data(), shader_binary.size() };

		d3d12_device_p->CreateComputePipelineState(
			&d3d12_pipeline_state_desc,
			IID_PPV_ARGS(&d3d12_pipeline_state_p)
		);
		NCPP_ASSERT(d3d12_pipeline_state_p) << "can't create d3d12 pipeline state";

		return d3d12_pipeline_state_p;
	}

}