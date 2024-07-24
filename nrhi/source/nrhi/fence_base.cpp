#include <nrhi/fence_base.hpp>



#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION
namespace nrhi {

	A_fence::A_fence(
		TKPA_valid<A_device> device_p,
		const F_fence_desc& desc
	) :
		A_device_child(device_p),
		desc_(desc)
	{
	}
	A_fence::~A_fence(){
	}

}
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION