#include <nrhi/directx12/compute_pipeline_state.hpp>
#include <nrhi/directx12/shader.hpp>



namespace nrhi {

	F_directx12_compute_pipeline_state::F_directx12_compute_pipeline_state(
		TKPA_valid<A_device> device_p,
		const F_pipeline_state_desc& desc,
		ED_pipeline_state_type overrided_type
	) :
		F_directx12_pipeline_state(device_p, desc, overrided_type)
	{
		NCPP_ASSERT(desc.shader_p_vector.size() == 1) << "invalid shaders";

		const auto& shader_p = desc.shader_p_vector[0];
		const auto& shader_desc = shader_p->desc();
		NCPP_ASSERT(shader_desc.type == ED_shader_type::COMPUTE) << "invalid shader type";

		d3d12_compute_shader_p_ = shader_p.T_cast<F_directx12_compute_shader>()->d3d12_compute_shader_p();
	}
	F_directx12_compute_pipeline_state::~F_directx12_compute_pipeline_state(){

	}

}