#include <nrhi/directx12/sampler_state.hpp>
#include <nrhi/directx12/device.hpp>
#include <nrhi/sampler_state.hpp>



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
#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_BINDING
	void F_directx12_sampler_state::rebuild(
		const F_sampler_state_desc& desc,
		const F_descriptor& descriptor
	) {
		finalize_rebuild(
			desc,
			descriptor
		);
	}
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_BINDING



#pragma region Alternative Functions
#ifdef NRHI_DRIVER_ENABLE_INTERFACE_ONLY_SUPPORTS
	TU<A_sampler_state> HD_directx12_sampler_state::create(
		TKPA_valid<A_device> device_p,
		const F_sampler_state_desc& desc
	) {
		return H_sampler_state::ALTERNATIVE::create(
			device_p,
			desc
		);
	}
#endif // NRHI_DRIVER_ENABLE_INTERFACE_ONLY_SUPPORTS
#pragma endregion

}