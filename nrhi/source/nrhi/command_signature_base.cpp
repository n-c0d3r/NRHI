#include <nrhi/command_signature_base.hpp>



#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_INDIRECT_COMMANDS
namespace nrhi {

	A_command_signature::A_command_signature(
		TKPA_valid<A_device> device_p,
		const F_command_signature_desc& desc,
		TKPA_valid<A_root_signature> root_signature_p
	) :
		A_root_signature_child(device_p, root_signature_p),
		desc_(desc)
	{
	}
	A_command_signature::~A_command_signature() {
	}

	void A_command_signature::rebuild(
		const F_command_signature_desc& desc
	) {
		finalize_rebuild(desc);
	}

	void A_command_signature::finalize_rebuild(
		const F_command_signature_desc& desc
	) {
		desc_ = desc;
	}

}
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_INDIRECT_COMMANDS