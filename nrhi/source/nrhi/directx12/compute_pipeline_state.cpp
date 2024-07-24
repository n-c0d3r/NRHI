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

		if(d3d12_pipeline_state_p_)
			d3d12_pipeline_state_p_->Release();
	}

	ID3D12PipelineState* F_directx12_compute_pipeline_state::create_d3d12_compute_pipeline_state_direct(
		TKPA_valid<A_device> device_p,
		const F_pipeline_state_desc& desc
	) {
		ID3D12Device* d3d12_device_p = device_p.T_cast<F_directx12_device>()->d3d12_device_p();
		ID3D12RootSignature* d3d12_root_signature_p = desc.root_signature_p.T_cast<F_directx12_root_signature>()->d3d12_root_signature_p();

		ID3D12PipelineState* d3d12_pipeline_state_p = 0;

		D3D12_COMPUTE_PIPELINE_STATE_DESC  d3d12_pipeline_state_desc = {};
		d3d12_pipeline_state_desc.pRootSignature = d3d12_root_signature_p;

		u32 shader_count = desc.direct_shader_descs.size();
		NCPP_ASSERT(shader_count) << "require compute shader";
		const auto& direct_shader_desc = desc.direct_shader_descs[0];
		NCPP_ASSERT(direct_shader_desc.type) << "invalid shader type";
		const auto& direct_shader_binary = direct_shader_desc.binary;
		d3d12_pipeline_state_desc.CS = { direct_shader_binary.data(), direct_shader_binary.size() };

		d3d12_device_p->CreateComputePipelineState(
			&d3d12_pipeline_state_desc,
			IID_PPV_ARGS(&d3d12_pipeline_state_p)
		);
		NCPP_ASSERT(d3d12_pipeline_state_p) << "can't create d3d12 pipeline state";

		return d3d12_pipeline_state_p;
	}

}