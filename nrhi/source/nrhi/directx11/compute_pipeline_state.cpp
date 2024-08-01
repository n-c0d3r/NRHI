#include <nrhi/directx11/compute_pipeline_state.hpp>
#include <nrhi/directx11/device.hpp>



namespace nrhi {

	F_directx11_compute_pipeline_state::F_directx11_compute_pipeline_state(
		TKPA_valid<A_device> device_p,
		const F_compute_pipeline_state_options& options
	) :
		F_directx11_pipeline_state(device_p, ED_pipeline_state_type::COMPUTE),
		options_(options)
	{
		const auto& shader_binary = options.shader_binaries.compute;

		auto d3d11_device_p = device_p.T_cast<F_directx11_device>()->d3d11_device_p();

		HRESULT hr = d3d11_device_p->CreateComputeShader(
			shader_binary.data(),
			shader_binary.size(),
			0,
			&d3d11_compute_shader_p_
		);

		NCPP_ASSERT(!FAILED(hr)) << "can't create d3d11 compute shader";
	}
	F_directx11_compute_pipeline_state::~F_directx11_compute_pipeline_state(){

		if(d3d11_compute_shader_p_)
			d3d11_compute_shader_p_->Release();
	}

}