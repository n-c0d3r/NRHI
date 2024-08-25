#include <nrhi/resource_view_base.hpp>
#include <nrhi/resource_base.hpp>



namespace nrhi {

    A_resource_view::A_resource_view(
        TKPA_valid<A_device> device_p,
        const F_resource_view_desc& desc,
        ED_resource_view_type overrided_type
    ) :
        A_device_child(device_p),
        desc_(desc),
		generation_(desc.resource_p->generation())
    {
        desc_.type = overrided_type;

		if(desc_.overrided_resource_type == ED_resource_type::NONE)
			resource_type_ = desc.resource_p->desc().type;
		else
			resource_type_ = desc_.overrided_resource_type;
    }
#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_BINDING
	A_resource_view::A_resource_view(
		TKPA_valid<A_device> device_p,
		const F_resource_view_desc& desc,
		const F_descriptor& managed_descriptor,
		ED_resource_view_type overrided_type
	) :
		A_device_child(device_p),
		desc_(desc),
		generation_(desc.resource_p->generation()),
		descriptor_(managed_descriptor)
	{
		desc_.type = overrided_type;

		if(desc_.overrided_resource_type == ED_resource_type::NONE)
			resource_type_ = desc.resource_p->desc().type;
		else
			resource_type_ = desc_.overrided_resource_type;
	}
	A_resource_view::A_resource_view(
		TKPA_valid<A_device> device_p,
		const F_descriptor_handle& descriptor_handle,
		ED_resource_view_type overrided_type
	) :
		A_device_child(device_p),
		descriptor_({ .handle = descriptor_handle })
    {
    	desc_.type = overrided_type;
    }
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_BINDING
    A_resource_view::~A_resource_view() {

    }

	void A_resource_view::rebuild() {

		finalize_rebuild();
	}
	void A_resource_view::rebuild(
		const F_resource_view_desc& desc
	) {
		finalize_rebuild(desc);
	}
#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_BINDING
	void A_resource_view::rebuild_with_managed_descriptor(
		const F_resource_view_desc& desc,
		const F_descriptor& descriptor
	) {
		finalize_rebuild_with_managed_descriptor(desc, descriptor);
    }
	void A_resource_view::rebuild_with_unmanaged_descriptor_handle(
		const F_descriptor_handle& unmanaged_descriptor_handle
	) {
    	finalize_rebuild_with_unmanaged_descriptor_handle(unmanaged_descriptor_handle);
    }
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_BINDING
	void A_resource_view::guarantee_generation() {

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_BINDING
    	NCPP_ASSERT(management_type() == E_resource_view_management_type::MANAGED);
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_BINDING

    	if(!is_valid_generation())
			rebuild();
	}
	void A_resource_view::finalize_rebuild() {

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_BINDING
    	NCPP_ASSERT(management_type() == E_resource_view_management_type::MANAGED);
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_BINDING

		generation_ = desc_.resource_p->generation();
	}
	void A_resource_view::finalize_rebuild(
		const F_resource_view_desc& desc
	) {
#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_BINDING
    	NCPP_ASSERT(management_type() == E_resource_view_management_type::MANAGED);
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_BINDING

    	NCPP_ASSERT(desc_.type == desc.type) << "can't change type";
		desc_ = desc;
		finalize_rebuild();
	}
#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_BINDING
	void A_resource_view::finalize_rebuild_with_managed_descriptor(
		const F_resource_view_desc& desc,
		const F_descriptor& managed_descriptor
	) {
#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_BINDING
    	NCPP_ASSERT(management_type() == E_resource_view_management_type::MANAGED);
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_BINDING

		descriptor_ = managed_descriptor;
		finalize_rebuild(desc);
    }
	void A_resource_view::finalize_rebuild_with_unmanaged_descriptor_handle(
		const F_descriptor_handle& unmanaged_descriptor_handle
	) {
#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_BINDING
    	NCPP_ASSERT(management_type() == E_resource_view_management_type::UNMANAGED);
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_BINDING

    	descriptor_.handle = unmanaged_descriptor_handle;
    }
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_BINDING

	void A_resource_view::release_driver_specific_implementation()
    {
#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_BINDING
    	descriptor_ = {};
#endif
    }

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_BINDING
	E_resource_view_management_type A_resource_view::management_type() const
    {
		return E_resource_view_management_type::MANAGED;
    }
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_BINDING
}