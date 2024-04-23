#include <nrhi/directx11/pipeline.hpp>



namespace nrhi {

	F_directx11_pipeline::F_directx11_pipeline(
		TK_valid<A_device> device_p,
		const F_pipeline_desc& desc,
		E_pipeline_type overrided_type
	) :
		A_pipeline(device_p, desc, overrided_type)
	{

	}

}