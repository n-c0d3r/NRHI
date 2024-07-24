#include <nrhi/root_signature_base.hpp>



#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_BINDING
namespace nrhi {

	A_root_signature::A_root_signature(
		TKPA_valid<A_device> device_p,
		const F_root_signature_desc& desc
	) :
		A_device_child(device_p),
		desc_(desc)
	{
	}
	A_root_signature::~A_root_signature() {
	}

	void A_root_signature::rebuild(
		const F_root_signature_desc& desc
	) {
		finalize_rebuild(desc);
	}

	void A_root_signature::finalize_rebuild(
		const F_root_signature_desc& desc
	) {
		desc_ = desc;
	}

}
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_BINDING