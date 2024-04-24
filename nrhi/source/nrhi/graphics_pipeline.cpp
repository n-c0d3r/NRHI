#include <nrhi/graphics_pipeline.hpp>
#include <nrhi/pipeline.hpp>



namespace nrhi {

	U_graphics_pipeline_handle H_graphics_pipeline::create(
		TK_valid<A_device> device_p,
		const F_pipeline_desc& desc
	) {

		return H_pipeline::create_graphics_pipeline(
			device_p,
			desc
		);
	}

}