#include <nrhi/root_signature_child.hpp>



#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_BINDING
namespace nrhi {

	A_root_signature_child::A_root_signature_child(
		TKPA_valid<A_device> device_p,
		TKPA<A_root_signature> root_signature_p
	) :
		A_device_child(device_p),
		root_signature_p_(root_signature_p)
	{
		NRHI_DRIVER_ENABLE_IF_SUPPORT_ADVANCED_RESOURCE_BINDING(
			NCPP_ASSERT(root_signature_p) << "root signature is invalid";
		);
	}
	A_root_signature_child::~A_root_signature_child() {
	}

}
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_BINDING