#include <nrhi/compute_pipeline_state.hpp>
#include <nrhi/pipeline_state.hpp>
#include <nrhi/driver.hpp>



namespace nrhi {

#ifdef NRHI_DRIVER_SUPPORT_SIMPLE_RESOURCE_BINDING
	U_compute_pipeline_state_handle H_compute_pipeline_state::create(
		TKPA_valid<A_device> device_p,
		const F_pipeline_state_desc& desc
	) {
		NRHI_DRIVER_REQUIRE_SUPPORT_SIMPLE_RESOURCE_BINDING(
			return H_pipeline_state::create_compute_pipeline_state(
				device_p,
				desc
			);
		);
	}
#endif // NRHI_DRIVER_SUPPORT_SIMPLE_RESOURCE_BINDING
#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_BINDING
	U_graphics_pipeline_state_handle H_graphics_pipeline_state::create_direct(
		TKPA_valid<A_device> device_p,
		const F_pipeline_state_desc& desc
	) {
		NRHI_DRIVER_REQUIRE_SUPPORT_ADVANCED_RESOURCE_BINDING(
			return H_pipeline_state::create_graphics_pipeline_state_direct(
				device_p,
				desc
			);
		);
	}
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_BINDING

}