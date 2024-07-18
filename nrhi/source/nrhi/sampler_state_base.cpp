#include <nrhi/sampler_state_base.hpp>



namespace nrhi {

	A_sampler_state::A_sampler_state(
		TKPA_valid<A_device> device_p,
		const F_sampler_state_desc& desc
	) :
		A_device_child(device_p),
		desc_(desc)
	{
	}
	A_sampler_state::~A_sampler_state() {
	}

	void A_sampler_state::rebuild(
		const F_sampler_state_desc& desc
	) {
		finalize_rebuild(desc);
	}
#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_BINDING
	void A_sampler_state::rebuild(
		const F_sampler_state_desc& desc,
		const F_descriptor& descriptor
	) {
		finalize_rebuild(
			desc,
			descriptor
		);
	}
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_BINDING

	void A_sampler_state::finalize_rebuild(
		const F_sampler_state_desc& desc
	) {
		desc_ = desc;
	}
#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_BINDING
	void A_sampler_state::finalize_rebuild(
		const F_sampler_state_desc& desc,
		const F_descriptor& descriptor
	) {
		desc_ = desc;
		descriptor_ = descriptor;
	}
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_BINDING

}