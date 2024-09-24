#include <nrhi/directx12/sampler_state.hpp>
#include <nrhi/directx12/device.hpp>
#include <nrhi/directx12/descriptor.hpp>
#include <nrhi/sampler_state.hpp>



namespace nrhi {

	F_directx12_sampler_state::F_directx12_sampler_state(
		TKPA_valid<A_device> device_p,
		const F_sampler_state_desc& desc,
		const F_descriptor& managed_descriptor
	) :
		A_sampler_state(device_p, desc, managed_descriptor)
	{
	}
	F_directx12_sampler_state::F_directx12_sampler_state(
		TKPA_valid<A_device> device_p,
		const F_descriptor_handle& unmanaged_descriptor_handle
	) :
		A_sampler_state(device_p, unmanaged_descriptor_handle)
	{
	}
	F_directx12_sampler_state::~F_directx12_sampler_state() {
	}

	void F_directx12_sampler_state::release_driver_specific_implementation()
	{
		A_sampler_state::release_driver_specific_implementation();
	}



	F_directx12_managed_sampler_state::F_directx12_managed_sampler_state(
		TKPA_valid<A_device> device_p,
		const F_sampler_state_desc& desc,
		const F_descriptor& managed_descriptor
	) :
		F_directx12_sampler_state(device_p, desc, managed_descriptor)
	{
	}
	F_directx12_managed_sampler_state::~F_directx12_managed_sampler_state() {

		if(managed_descriptor())
			F_directx12_managed_sampler_state::release_driver_specific_implementation();
	}

	void F_directx12_managed_sampler_state::rebuild(
		const F_sampler_state_desc& desc
	) {
		NCPP_ASSERT(managed_descriptor()) << "invalid descriptor";

		HD_directx12_descriptor::initialize_sampler_state(
			device_p(),
			managed_descriptor().handle.cpu_address,
			desc
		);

		finalize_rebuild(
			desc
		);
	}
	void F_directx12_managed_sampler_state::rebuild_with_managed_descriptor(
		const F_sampler_state_desc& desc,
		const F_descriptor& descriptor
	) {
		if(managed_descriptor())
			F_directx12_managed_sampler_state::release_driver_specific_implementation();

		NCPP_ASSERT(descriptor) << "invalid descriptor";

		HD_directx12_descriptor::initialize_sampler_state(
			device_p(),
			descriptor.handle.cpu_address,
			desc
		);

		finalize_rebuild_with_managed_descriptor(
			desc,
			descriptor
		);
	}



	F_directx12_unmanaged_sampler_state::F_directx12_unmanaged_sampler_state(
		TKPA_valid<A_device> device_p,
		const F_descriptor_handle& unmanaged_descriptor_handle
	) :
		F_directx12_sampler_state(device_p, unmanaged_descriptor_handle)
	{
	}
	F_directx12_unmanaged_sampler_state::~F_directx12_unmanaged_sampler_state() {
	}

	E_sampler_state_management_type F_directx12_unmanaged_sampler_state::management_type() const
	{
		return E_sampler_state_management_type::UNMANAGED;
	}




	TU<A_sampler_state> HD_directx12_sampler_state::create_with_managed_descriptor(
		TKPA_valid<A_device> device_p,
		const F_sampler_state_desc& desc,
		const F_descriptor& managed_descriptor
	)
	{
		return TU<F_directx12_managed_sampler_state>()(
			device_p,
			desc,
			managed_descriptor
		);
	}

	TU<A_sampler_state> HD_directx12_sampler_state::create_with_unmanaged_descriptor_handle(
		TKPA_valid<A_device> device_p,
		const F_descriptor_handle& unmanaged_descriptor_handle
	)
	{
		return TU<F_directx12_unmanaged_sampler_state>()(
			device_p,
			unmanaged_descriptor_handle
		);
	}

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