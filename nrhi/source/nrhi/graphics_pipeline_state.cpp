#include <nrhi/graphics_pipeline_state.hpp>
#include <nrhi/pipeline_state.hpp>



namespace nrhi {

	U_graphics_pipeline_state_handle H_graphics_pipeline_state::create(
		TKPA_valid<A_device> device_p,
		const F_pipeline_state_desc& desc
	) {

		return H_pipeline_state::create_graphics_pipeline_state(
			device_p,
			desc
		);
	}

}