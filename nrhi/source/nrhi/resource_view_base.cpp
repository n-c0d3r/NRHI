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
		const F_descriptor& descriptor,
		ED_resource_view_type overrided_type
	) :
		A_device_child(device_p),
		desc_(desc),
		generation_(desc.resource_p->generation()),
		descriptor_(descriptor)
	{
		desc_.type = overrided_type;

		if(desc_.overrided_resource_type == ED_resource_type::NONE)
			resource_type_ = desc.resource_p->desc().type;
		else
			resource_type_ = desc_.overrided_resource_type;
	}
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_BINDING
    A_resource_view::~A_resource_view() {

    }

	void A_resource_view::rebuild() {

		finalize_rebuild();
	}
	void A_resource_view::guarantee_generation() {

		if(!is_valid_generation())
			rebuild();
	}
	void A_resource_view::finalize_rebuild() {

		generation_ = desc_.resource_p->generation();
	}

}