#include <nrhi/directx11/compute_pipeline_state.hpp>



namespace nrhi {

	F_directx11_compute_pipeline_state::F_directx11_compute_pipeline_state(
		TK_valid<A_device> device_p,
		const F_pipeline_state_desc& desc,
		E_pipeline_state_type overrided_type
	) :
		F_directx11_pipeline_state(device_p, desc, overrided_type)
	{

	}
	F_directx11_compute_pipeline_state::~F_directx11_compute_pipeline_state(){

	}

}