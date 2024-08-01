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
#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
	void A_resource::rebuild_committed(
		const F_resource_desc& desc
	) {
		finalize_rebuild_committed(desc);
	}
	void A_resource::rebuild_placed(
		const F_resource_desc& desc,
		TKPA_valid<A_resource_heap> heap_p,
		u64 heap_offset
	) {
		finalize_rebuild_placed(
			desc,
			heap_p,
			heap_offset
		);
	}
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
	void A_resource::finalize_rebuild(
		const F_initial_resource_data& initial_data,
		const F_resource_desc& desc
	) {
		NRHI_DRIVER_REQUIRE_SUPPORT_SIMPLE_RESOURCE_MANAGEMENT(
			initial_data_ = initial_data;
			desc_ = desc;
			++generation_;
		);
	}
#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
	void A_resource::finalize_rebuild_committed(
		const F_resource_desc& desc
	) {
		NCPP_ASSERT(management_type() == E_resource_management_type::COMMITTED);
		desc_ = desc;
	}
	void A_resource::finalize_rebuild_placed(
		const F_resource_desc& desc,
		TKPA_valid<A_resource_heap> heap_p,
		u64 heap_offset
	) {
		NCPP_ASSERT(management_type() == E_resource_management_type::PLACED);
		desc_ = desc;
		memory_p_ = heap_p.no_requirements();
		heap_offset_ = heap_offset;
	}
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
	E_resource_management_type A_resource::management_type() const {

		NRHI_DRIVER_REQUIRE_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT(
			return E_resource_management_type::NONE;
		);
	}
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT

}