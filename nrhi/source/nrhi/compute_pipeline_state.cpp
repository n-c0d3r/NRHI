#include <nrhi/compute_pipeline_state.hpp>
#include <nrhi/pipeline_state.hpp>



namespace nrhi {

	U_compute_pipeline_state_handle H_compute_pipeline_state::create(
		TKPA_valid<A_device> device_p,
		const F_pipeline_state_desc& desc
	) {

		return H_pipeline_state::create_compute_pipeline_state(
			device_p,
			desc
		);
	}

}