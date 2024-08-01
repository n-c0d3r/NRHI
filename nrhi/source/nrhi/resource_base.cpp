#include <nrhi/resource_base.hpp>
#include <nrhi/driver.hpp>



namespace nrhi {

    A_resource::A_resource(
        TKPA_valid<A_device> device_p,
        const F_initial_resource_data& initial_data,
        const F_resource_desc& desc,
        ED_resource_type overrided_type
    ) :
        A_device_child(device_p),
        desc_(desc),
        initial_data_(initial_data)
    {

        desc_.type = overrided_type;
    }
    A_resource::~A_resource() {

    }

	void A_resource::rebuild(
		const F_initial_resource_data& initial_data,
		const F_resource_desc& desc
	) {
		finalize_rebuild(
			initial_data,
			desc
		);
	}
	void A_resource::finalize_rebuild(
		const F_initial_resource_data& initial_data,
		const F_resource_desc& desc
	) {
		initial_data_ = initial_data;
		desc_ = desc;
		++generation_;
	}

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
	E_resource_management_type A_resource::management_type() const {

		NRHI_DRIVER_REQUIRE_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT(
			return E_resource_management_type::NONE;
		);
	}
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT

}