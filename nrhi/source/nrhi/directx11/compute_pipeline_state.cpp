#include <nrhi/directx11/compute_pipeline_state.hpp>
#include <nrhi/directx11/shader.hpp>
#include <nrhi/directx11/shader_blob.hpp>



namespace nrhi {

	F_directx11_compute_pipeline_state::F_directx11_compute_pipeline_state(
		TKPA_valid<A_device> device_p,
		const F_pipeline_state_desc& desc,
		E_pipeline_state_type overrided_type
	) :
		F_directx11_pipeline_state(device_p, desc, overrided_type)
	{

		NCPP_ASSERT(desc.shader_p_vector.size() == 1) << "invalid shaders";

		const auto& shader_p = desc.shader_p_vector[0];
		const auto& shader_blob_p = shader_p->desc().blob_p;
		NCPP_ASSERT(shader_blob_p->desc().type == E_shader_type::COMPUTE) << "invalid shader type";

		d3d11_compute_shader_p_ = shader_p.T_cast<F_directx11_compute_shader>()->d3d11_compute_shader_p();
	}
	F_directx11_compute_pipeline_state::~F_directx11_compute_pipeline_state(){

	}

}