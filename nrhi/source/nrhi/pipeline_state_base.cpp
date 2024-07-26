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
	A_pipeline_state::~A_pipeline_state(){

	}

}