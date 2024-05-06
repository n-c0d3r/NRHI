#include <nrhi/resource_base.hpp>



namespace nrhi {

    A_resource::A_resource(
        TKPA_valid<A_device> device_p,
        const F_initial_resource_data& initial_data,
        const F_resource_desc& desc,
        E_resource_type overrided_type
    ) :
        device_p_(device_p),
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

}