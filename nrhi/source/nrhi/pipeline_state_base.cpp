#include <nrhi/pipeline_state_base.hpp>



namespace nrhi {

	A_pipeline_state::A_pipeline_state(
		TKPA_valid<A_device> device_p,
		ED_pipeline_state_type type
	) :
		A_device_child(device_p),
		type_(type)
	{
	}
#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_BINDING
	A_pipeline_state::A_pipeline_state(
		TKPA_valid<A_device> device_p,
		TKPA_valid<A_root_signature> root_signature_p,
		ED_pipeline_state_type type
	) :
		A_device_child(device_p),
		type_(type),
		root_signature_p_(root_signature_p.no_requirements())
	{
	}
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_BINDING
	A_pipeline_state::~A_pipeline_state(){
	}

}