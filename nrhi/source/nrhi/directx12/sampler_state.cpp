#include <nrhi/directx12/sampler_state.hpp>
#include <nrhi/directx12/device.hpp>



namespace nrhi {

	F_directx12_sampler_state::F_directx12_sampler_state(
		TKPA_valid<A_device> device_p,
		const F_sampler_state_desc& desc
	) :
		A_sampler_state(device_p, desc)
	{
	}
	F_directx12_sampler_state::F_directx12_sampler_state(
		TKPA_valid<A_device> device_p,
		const F_sampler_state_desc& desc,
		const F_descriptor& descriptor
	) :
		A_sampler_state(device_p, desc, descriptor)
	{
	}
	F_directx12_sampler_state::~F_directx12_sampler_state() {
	}

	void F_directx12_sampler_state::rebuild(
		const F_sampler_state_desc& desc
	) {
		finalize_rebuild(desc);
	}

}