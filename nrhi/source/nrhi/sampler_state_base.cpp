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
#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_BINDING
	A_sampler_state::A_sampler_state(
		TKPA_valid<A_device> device_p,
		const F_sampler_state_desc& desc,
		const F_descriptor& managed_descriptor
	) :
		A_device_child(device_p),
		desc_(desc),
		descriptor_(managed_descriptor)
	{
	}
	A_sampler_state::A_sampler_state(
		TKPA_valid<A_device> device_p,
		const F_descriptor_handle& unmanaged_descriptor_handle
	) :
		A_device_child(device_p),
		descriptor_({ .handle = unmanaged_descriptor_handle })
	{
	}
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_BINDING
	A_sampler_state::~A_sampler_state() {
	}

	void A_sampler_state::rebuild(
		const F_sampler_state_desc& desc
	) {
		finalize_rebuild(desc);
	}
#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_BINDING
	void A_sampler_state::rebuild_with_managed_descriptor(
		const F_sampler_state_desc& desc,
		const F_descriptor& managed_descriptor
	) {
		finalize_rebuild_with_managed_descriptor(
			desc,
			managed_descriptor
		);
	}
	void A_sampler_state::rebuild_with_unmanaged_descriptor_handle(
		const F_descriptor_handle& unmanaged_descriptor_handle
	) {
		finalize_rebuild_with_unmanaged_descriptor_handle(
			unmanaged_descriptor_handle
		);
	}
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_BINDING

	void A_sampler_state::finalize_rebuild(
		const F_sampler_state_desc& desc
	) {
#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_BINDING
		NCPP_ASSERT(management_type() == E_sampler_state_management_type::MANAGED);
#endif

		desc_ = desc;
	}
#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_BINDING
	void A_sampler_state::finalize_rebuild_with_managed_descriptor(
		const F_sampler_state_desc& desc,
		const F_descriptor& managed_descriptor
	) {
		NCPP_ASSERT(management_type() == E_sampler_state_management_type::MANAGED);

		desc_ = desc;
		descriptor_ = managed_descriptor;
	}
	void A_sampler_state::finalize_rebuild_with_unmanaged_descriptor_handle(
		const F_descriptor_handle& unmanaged_descriptor_handle
	) {
		NCPP_ASSERT(management_type() == E_sampler_state_management_type::UNMANAGED);

		descriptor_.handle = unmanaged_descriptor_handle;
	}
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_BINDING

	void A_sampler_state::release_driver_specific_implementation()
	{
#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_BINDING
    	descriptor_ = {};
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_BINDING
	}

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_BINDING
	E_sampler_state_management_type A_sampler_state::management_type() const
	{
		return E_sampler_state_management_type::MANAGED;
	}
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_BINDING
}