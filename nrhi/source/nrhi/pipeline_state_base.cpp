#include <nrhi/pipeline_state_base.hpp>



namespace nrhi {

	A_pipeline_state::A_pipeline_state(
		TK_valid<A_device> device_p,
		const F_pipeline_state_desc& desc,
		E_pipeline_state_type overrided_type
	) :
		device_p_(device_p),
		desc_(desc)
	{
		desc_.type = overrided_type;
	}
	A_pipeline_state::~A_pipeline_state(){

	}

}