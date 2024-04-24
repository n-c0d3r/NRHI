#include <nrhi/directx11/pipeline.hpp>
#include <nrhi/directx11/graphics_pipeline.hpp>
#include <nrhi/directx11/compute_pipeline.hpp>



namespace nrhi {

	F_directx11_pipeline::F_directx11_pipeline(
		TK_valid<A_device> device_p,
		const F_pipeline_desc& desc,
		E_pipeline_type overrided_type
	) :
		A_pipeline(device_p, desc, overrided_type)
	{
	}
	F_directx11_pipeline::~F_directx11_pipeline()
	{
	}



	TU<A_pipeline> HD_directx11_pipeline::create(
		TK_valid<A_device> device_p,
		const F_pipeline_desc& desc
	) {

		switch (desc.type){

		case E_pipeline_type::GRAPHICS:
			return TU<F_directx11_graphics_pipeline>()(device_p, desc);
		}
	}

	U_graphics_pipeline_handle HD_directx11_pipeline::create_graphics_pipeline(
		TK_valid<A_device> device_p,
		const F_pipeline_desc& desc
	) {

		return { TU<F_directx11_graphics_pipeline>()(device_p, desc) };
	}

}