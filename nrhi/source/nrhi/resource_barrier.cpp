#include <nrhi/resource_barrier.hpp>

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION
namespace nrhi {

	F_resource_barrier H_resource_barrier::transition(
		const F_resource_transition_barrier& transition_barrier,
		ED_resource_barrier_flag flags
	) {
		F_resource_barrier result;
		result.type = ED_resource_barrier_type::TRANSITION;
		result.flags = flags;
		result.transition = transition_barrier;

		return result;
	}
	F_resource_barrier H_resource_barrier::aliasing(
		const F_resource_aliasing_barrier& aliasing_barrier,
		ED_resource_barrier_flag flags
	) {
		F_resource_barrier result;
		result.type = ED_resource_barrier_type::ALIASING;
		result.flags = flags;
		result.aliasing = aliasing_barrier;

		return result;
	}
	F_resource_barrier H_resource_barrier::uav(
		const F_resource_uav_barrier& uav_barrier,
		ED_resource_barrier_flag flags
	) {
		F_resource_barrier result;
		result.type = ED_resource_barrier_type::UAV;
		result.flags = flags;
		result.uav = uav_barrier;

		return result;
	}

}
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION