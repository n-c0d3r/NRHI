#include <nrhi/directx11/graphics_pipeline.hpp>



namespace nrhi {

	F_directx11_graphics_pipeline::F_directx11_graphics_pipeline(
		TK_valid<A_device> device_p,
		const F_pipeline_desc& desc,
		E_pipeline_type overrided_type
	) :
		F_directx11_pipeline(device_p, desc, overrided_type)
	{

	}
	F_directx11_graphics_pipeline::~F_directx11_graphics_pipeline(){

	}

}