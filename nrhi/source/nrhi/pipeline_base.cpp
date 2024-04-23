#include <nrhi/pipeline_base.hpp>



namespace nrhi {

	A_pipeline::A_pipeline(
		TK_valid<A_device> device_p,
		const F_pipeline_desc& desc,
		E_pipeline_type overrided_type
	) :
		device_p_(device_p),
		desc_(desc)
	{
		desc_.type = overrided_type;
	}
	A_pipeline::~A_pipeline(){

	}

}