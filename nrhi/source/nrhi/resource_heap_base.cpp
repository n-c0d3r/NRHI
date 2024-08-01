#include <nrhi/resource_heap_base.hpp>



#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
namespace nrhi {

	A_resource_heap::A_resource_heap(
		TKPA_valid<A_device> device_p,
		const F_resource_heap_desc& desc
	) :
		A_device_child(device_p),
		desc_(desc)
	{
	}
	A_resource_heap::~A_resource_heap() {
	}

	void A_resource_heap::rebuild(
		const F_resource_heap_desc& desc
	) {
		finalize_rebuild(desc);
	}

	void A_resource_heap::finalize_rebuild(
		const F_resource_heap_desc& desc
	) {
		desc_ = desc;
	}

}
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT